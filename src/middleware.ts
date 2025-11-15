import type { Context, Next } from "hono";
import type { Bindings } from "./app";

/**
 * Cloudflare Access JWT Configuration
 */
interface CloudflareAccessConfig {
	/** The team domain (e.g., 'h-2883.cloudflareaccess.com') */
	teamDomain: string;
	/** The policy AUD tag from Cloudflare Access */
	policyAud: string;
}

/**
 * JWT Header structure
 */
interface JWTHeader {
	kid: string;
	alg: string;
}

/**
 * JWT Payload structure from Cloudflare Access
 */
interface JWTPayload {
	iss: string;
	sub: string;
	aud: string[];
	exp: number;
	iat: number;
	nonce?: string;
	email?: string;
	type?: string;
	identity_nonce?: string;
	custom?: Record<string, unknown>;
}

/**
 * JWKS (JSON Web Key Set) structure
 */
interface JWKS {
	keys: JWK[];
}

/**
 * JWK (JSON Web Key) structure
 */
interface JWK {
	kid: string;
	kty: string;
	alg: string;
	use: string;
	n: string;
	e: string;
}

/**
 * Cache for JWKS keys (to avoid fetching on every request)
 */
let jwksCache: JWKS | null = null;
let jwksCacheTime = 0;
const JWKS_CACHE_TTL = 3600000; // 1 hour in milliseconds

/**
 * Fetches JWKS from Cloudflare Access
 */
async function fetchJWKS(teamDomain: string): Promise<JWKS> {
	const now = Date.now();

	// Return cached JWKS if still valid
	if (jwksCache && (now - jwksCacheTime) < JWKS_CACHE_TTL) {
		return jwksCache;
	}

	const certsUrl = `https://${teamDomain}/cdn-cgi/access/certs`;
	const response = await fetch(certsUrl);

	if (!response.ok) {
		throw new Error(`Failed to fetch JWKS: ${response.statusText}`);
	}

	const jwks = await response.json() as JWKS;

	// Update cache
	jwksCache = jwks;
	jwksCacheTime = now;

	return jwks;
}

/**
 * Converts a base64url string to ArrayBuffer
 */
function base64UrlToArrayBuffer(base64url: string): ArrayBuffer {
	// Add padding if needed
	const padding = '='.repeat((4 - (base64url.length % 4)) % 4);
	const base64 = base64url.replace(/-/g, '+').replace(/_/g, '/') + padding;

	const binaryString = atob(base64);
	const bytes = new Uint8Array(binaryString.length);

	for (let i = 0; i < binaryString.length; i++) {
		bytes[i] = binaryString.charCodeAt(i);
	}

	return bytes.buffer;
}

/**
 * Imports a JWK as a CryptoKey for verification
 */
async function importJWK(jwk: JWK): Promise<CryptoKey> {
	return await crypto.subtle.importKey(
		"jwk",
		{
			kty: jwk.kty,
			n: jwk.n,
			e: jwk.e,
			alg: jwk.alg,
			ext: true,
		},
		{
			name: "RSASSA-PKCS1-v1_5",
			hash: "SHA-256",
		},
		false,
		["verify"]
	);
}

/**
 * Decodes and parses a JWT without verification
 */
function decodeJWT(token: string): { header: JWTHeader; payload: JWTPayload; signature: string } {
	const parts = token.split('.');

	if (parts.length !== 3) {
		throw new Error('Invalid JWT format');
	}

	const header = JSON.parse(atob(parts[0].replace(/-/g, '+').replace(/_/g, '/'))) as JWTHeader;
	const payload = JSON.parse(atob(parts[1].replace(/-/g, '+').replace(/_/g, '/'))) as JWTPayload;

	return {
		header,
		payload,
		signature: parts[2],
	};
}

/**
 * Verifies a Cloudflare Access JWT
 */
async function verifyCloudflareAccessJWT(
	token: string,
	config: CloudflareAccessConfig
): Promise<JWTPayload> {
	// Decode the JWT
	const { header, payload } = decodeJWT(token);

	// Fetch JWKS
	const jwks = await fetchJWKS(config.teamDomain);

	// Find the matching key
	const jwk = jwks.keys.find((key) => key.kid === header.kid);

	if (!jwk) {
		throw new Error(`No matching key found for kid: ${header.kid}`);
	}

	// Import the public key
	const publicKey = await importJWK(jwk);

	// Verify the signature
	const parts = token.split('.');
	const signatureData = `${parts[0]}.${parts[1]}`;
	const signatureBytes = base64UrlToArrayBuffer(parts[2]);

	const encoder = new TextEncoder();
	const data = encoder.encode(signatureData);

	const isValid = await crypto.subtle.verify(
		"RSASSA-PKCS1-v1_5",
		publicKey,
		signatureBytes,
		data
	);

	if (!isValid) {
		throw new Error('Invalid JWT signature');
	}

	// Verify claims
	const now = Math.floor(Date.now() / 1000);

	// Check expiration
	if (payload.exp < now) {
		throw new Error('JWT has expired');
	}

	// Check issuer
	const expectedIssuer = `https://${config.teamDomain}`;
	if (payload.iss !== expectedIssuer) {
		throw new Error(`Invalid issuer. Expected ${expectedIssuer}, got ${payload.iss}`);
	}

	// Check audience
	if (!payload.aud || !payload.aud.includes(config.policyAud)) {
		throw new Error(`Invalid audience. Expected ${config.policyAud}`);
	}

	return payload;
}

/**
 * Hono middleware for Cloudflare Access JWT validation
 *
 * This middleware validates the CF-Access-JWT-Assertion header
 * and ensures only authenticated Cloudflare Access users can access the application.
 *
 * @param config - Cloudflare Access configuration
 */
export function cloudflareAccessMiddleware(config: CloudflareAccessConfig) {
	return async (c: Context<{ Bindings: Bindings }>, next: Next) => {
		// Get the JWT from the CF-Access-JWT-Assertion header
		const jwt = c.req.header('CF-Access-JWT-Assertion');

		if (!jwt) {
			return c.json(
				{
					error: 'Unauthorized',
					message: 'Missing CF-Access-JWT-Assertion header. Please ensure Cloudflare Access is properly configured.',
				},
				401
			);
		}

		try {
			// Verify the JWT
			const payload = await verifyCloudflareAccessJWT(jwt, config);

			// Store the validated payload in context for use in handlers
			c.set('cloudflareAccessPayload', payload);
			c.set('userEmail', payload.email || payload.sub);

			// Continue to the next handler
			await next();
		} catch (error) {
			console.error('Cloudflare Access JWT validation failed:', error);

			return c.json(
				{
					error: 'Unauthorized',
					message: error instanceof Error ? error.message : 'Invalid or expired authentication token',
				},
				401
			);
		}
	};
}

/**
 * Helper function to get the validated user from context
 */
export function getCloudflareAccessUser(c: Context): JWTPayload | undefined {
	return c.get('cloudflareAccessPayload') as JWTPayload | undefined;
}
