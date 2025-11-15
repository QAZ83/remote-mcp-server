/**
 * Cloudflare Access JWKS (JSON Web Key Set) Configuration
 *
 * This file contains RSA public keys and certificates used for verifying
 * JWT tokens from Cloudflare Access.
 *
 * Generated: 2025-11-15
 */

export interface JWK {
	kid: string;
	kty: string;
	alg: string;
	use: string;
	e: string;
	n: string;
}

export interface Certificate {
	kid: string;
	cert: string;
}

export interface CloudflareAccessJWKS {
	keys: JWK[];
	public_cert: Certificate;
	public_certs: Certificate[];
}

/**
 * Cloudflare Access RSA public keys for JWT verification
 */
export const cloudflareAccessJWKS: CloudflareAccessJWKS = {
	keys: [
		{
			kid: "fca035a37fed690288402e5e0ce8fbacd846fb011b393bb497c0887c077b0f06",
			kty: "RSA",
			alg: "RS256",
			use: "sig",
			e: "AQAB",
			n: "tlAE-SUjxHCEb95787Xje5VyiCeDg86dJyMwnApliX8alU1jxNbQfxxRfEI_yjKjVKz-zRsqtZvbHBN0KOwfDAcSaL8QtgL2z-kL_u6NXA7NS4DbY-EAaMdxPin7RIG6I8W77bcl4N6O32PtoWNSw7UUWnhWmcdNW3Av6J0jFWBjSDykY-dmC6UzSuWWzurrvtUmWZlw0rpWXEjYL3EwXnHwTh-luAIykJbkO12ndqWh1zMoY1Lb9OBA1GwNbuDn4S-7rqwrSKY5wD365sxumnH8Ws0-G0G_9ukKZY3_kuQxF9HlLZc0YW63sBEuo-v0ocvf2IwyVtWWvwWR5cmDdQ",
		},
		{
			kid: "59659d34818532fdc3383571f513b7717b6277497124671f57940c45213dfbc1",
			kty: "RSA",
			alg: "RS256",
			use: "sig",
			e: "AQAB",
			n: "3Ibdb8qbUkajh-JtB1Pp6hUxlKjRtbI7e1ZujzJhxv_C10SOr2TcH6VklDc5p_RBDdIdk4Z8-2ukBgGSNbBrBpTFMiV6pGYe5zq9SdwQqXkq8g2DNe5ZulthMGwqSTtAGDxcaw2IgQ8gcZVq_aaYI4NyBZHvYTolOxsPhGUfWDxtTFSnmas-WKdQW_znA2zpcTter4t39Lx28Ss8MGQu3a3_xB6o2iZNEPhvkNqELEOEY00E-_ZmyxwT6tapKF2B1_Q5SlI2R-97aSoZ3vwjr7SrWTXMN8wq_1p67RTWXoYhF31p6AD7-cjO7zg6keZWLOsVrBivqhuljEDTkDmGGQ",
		},
	],
	public_cert: {
		kid: "59659d34818532fdc3383571f513b7717b6277497124671f57940c45213dfbc1",
		cert: `-----BEGIN CERTIFICATE-----
MIIDTTCCAjWgAwIBAgIRAMVOcctnBhDxD/0VxjsmJMgwDQYJKoZIhvcNAQELBQAw
YjELMAkGA1UEBhMCVVMxDjAMBgNVBAgTBVRleGFzMQ8wDQYDVQQHEwZBdXN0aW4x
EzARBgNVBAoTCkNsb3VkZmxhcmUxHTAbBgNVBAMTFGNsb3VkZmxhcmVhY2Nlc3Mu
Y29tMB4XDTI1MTExNTAzMjAxNloXDTI2MTEyOTAzMjAxNlowYjELMAkGA1UEBhMC
VVMxDjAMBgNVBAgTBVRleGFzMQ8wDQYDVQQHEwZBdXN0aW4xEzARBgNVBAoTCkNs
b3VkZmxhcmUxHTAbBgNVBAMTFGNsb3VkZmxhcmVhY2Nlc3MuY29tMIIBIjANBgkq
hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA3Ibdb8qbUkajh+JtB1Pp6hUxlKjRtbI7
e1ZujzJhxv/C10SOr2TcH6VklDc5p/RBDdIdk4Z8+2ukBgGSNbBrBpTFMiV6pGYe
5zq9SdwQqXkq8g2DNe5ZulthMGwqSTtAGDxcaw2IgQ8gcZVq/aaYI4NyBZHvYTol
OxsPhGUfWDxtTFSnmas+WKdQW/znA2zpcTter4t39Lx28Ss8MGQu3a3/xB6o2iZN
EPhvkNqELEOEY00E+/ZmyxwT6tapKF2B1/Q5SlI2R+97aSoZ3vwjr7SrWTXMN8wq
/1p67RTWXoYhF31p6AD7+cjO7zg6keZWLOsVrBivqhuljEDTkDmGGQIDAQABMA0G
CSqGSIb3DQEBCwUAA4IBAQBG+lN+HnrfgS938IGE4CyUNgLDDHdGsHCIqZfVTk3a
L/BQaI9E0KEDydRhQ6AOKTBerzLlpy5NHqhuNEBZ0PHBWg+p/u2i/RM4TIQbHMnK
OKQz9npXE/2+soCDBi2jZNaEgChRExATvRw3msC212663WrpVxPZRvw/cRmuq3BQ
itT4UHwY0C/Bzhk3PSTGbMU902xN3b7UhQec81Sy1aclJ1ARSoX7n9plD02vVTOp
dGwmKhnwS7DAlno+XOd/NVrfL0WABrtGrtdTyX/x0e9Qy5bkGOLrCm/8yph+cmKE
GlYmNchZccUcGoUpEWr7QnJzVpna1rPkUQ3tf2C2MC4z
-----END CERTIFICATE-----`,
	},
	public_certs: [
		{
			kid: "fca035a37fed690288402e5e0ce8fbacd846fb011b393bb497c0887c077b0f06",
			cert: `-----BEGIN CERTIFICATE-----
MIIDTTCCAjWgAwIBAgIRAJZULT6psHogHGFtysi9V7YwDQYJKoZIhvcNAQELBQAw
YjELMAkGA1UEBhMCVVMxDjAMBgNVBAgTBVRleGFzMQ8wDQYDVQQHEwZBdXN0aW4x
EzARBgNVBAoTCkNsb3VkZmxhcmUxHTAbBgNVBAMTFGNsb3VkZmxhcmVhY2Nlc3Mu
Y29tMB4XDTI1MTExNTAzMjAxNloXDTI2MTEyOTAzMjAxNlowYjELMAkGA1UEBhMC
VVMxDjAMBgNVBAgTBVRleGFzMQ8wDQYDVQQHEwZBdXN0aW4xEzARBgNVBAoTCkNs
b3VkZmxhcmUxHTAbBgNVBAMTFGNsb3VkZmxhcmVhY2Nlc3MuY29tMIIBIjANBgkq
hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtlAE+SUjxHCEb95787Xje5VyiCeDg86d
JyMwnApliX8alU1jxNbQfxxRfEI/yjKjVKz+zRsqtZvbHBN0KOwfDAcSaL8QtgL2
z+kL/u6NXA7NS4DbY+EAaMdxPin7RIG6I8W77bcl4N6O32PtoWNSw7UUWnhWmcdN
W3Av6J0jFWBjSDykY+dmC6UzSuWWzurrvtUmWZlw0rpWXEjYL3EwXnHwTh+luAIy
kJbkO12ndqWh1zMoY1Lb9OBA1GwNbuDn4S+7rqwrSKY5wD365sxumnH8Ws0+G0G/
9ukKZY3/kuQxF9HlLZc0YW63sBEuo+v0ocvf2IwyVtWWvwWR5cmDdQIDAQABMA0G
CSqGSIb3DQEBCwUAA4IBAQB3cnRhropPtoj+Cs3yGKFGf03d7TOn1vcekk7yZ+YG
cTI6MPwyaxVq+TTaLgT60M+H44Nd1kuxJe6dEUByfyK6EM++pW0zFVNN9v2OtQdK
d2ARA/0GQ/CosEXynLlrva8xWSfCl4cBHAxpk8HmDU21B8YSn1gYpb0/IU0qOBbb
PAYOVGUN57lPOqM17uXNFu+hHvsd96iR9cmV9B3KWdCFS7p1sY+MtBnVuLSWKDM
tOb+xJYhU1dwx7Fwi8SuQS48cSD8JR7QKpeeJqBDb0XJFYfrd0ynQiBpDOlgbIy5
eBJlOG9QtVgNEePo/ElSn/hqIYyQSnHYbj37YfA07GEy
-----END CERTIFICATE-----`,
		},
		{
			kid: "59659d34818532fdc3383571f513b7717b6277497124671f57940c45213dfbc1",
			cert: `-----BEGIN CERTIFICATE-----
MIIDTTCCAjWgAwIBAgIRAMVOcctnBhDxD/0VxjsmJMgwDQYJKoZIhvcNAQELBQAw
YjELMAkGA1UEBhMCVVMxDjAMBgNVBAgTBVRleGFzMQ8wDQYDVQQHEwZBdXN0aW4x
EzARBgNVBAoTCkNsb3VkZmxhcmUxHTAbBgNVBAMTFGNsb3VkZmxhcmVhY2Nlc3Mu
Y29tMB4XDTI1MTExNTAzMjAxNloXDTI2MTEyOTAzMjAxNlowYjELMAkGA1UEBhMC
VVMxDjAMBgNVBAgTBVRleGFzMQ8wDQYDVQQHEwZBdXN0aW4xEzARBgNVBAoTCkNs
b3VkZmxhcmUxHTAbBgNVBAMTFGNsb3VkZmxhcmVhY2Nlc3MuY29tMIIBIjANBgkq
hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA3Ibdb8qbUkajh+JtB1Pp6hUxlKjRtbI7
e1ZujzJhxv/C10SOr2TcH6VklDc5p/RBDdIdk4Z8+2ukBgGSNbBrBpTFMiV6pGYe
5zq9SdwQqXkq8g2DNe5ZulthMGwqSTtAGDxcaw2IgQ8gcZVq/aaYI4NyBZHvYTol
OxsPhGUfWDxtTFSnmas+WKdQW/znA2zpcTter4t39Lx28Ss8MGQu3a3/xB6o2iZN
EPhvkNqELEOEY00E+/ZmyxwT6tapKF2B1/Q5SlI2R+97aSoZ3vwjr7SrWTXMN8wq
/1p67RTWXoYhF31p6AD7+cjO7zg6keZWLOsVrBivqhuljEDTkDmGGQIDAQABMA0G
CSqGSIb3DQEBCwUAA4IBAQBG+lN+HnrfgS938IGE4CyUNgLDDHdGsHCIqZfVTk3a
L/BQaI9E0KEDydRhQ6AOKTBerzLlpy5NHqhuNEBZ0PHBWg+p/u2i/RM4TIQbHMnK
OKQz9npXE/2+soCDBi2jZNaEgChRExATvRw3msC212663WrpVxPZRvw/cRmuq3BQ
itT4UHwY0C/Bzhk3PSTGbMU902xN3b7UhQec81Sy1aclJ1ARSoX7n9plD02vVTOp
dGwmKhnwS7DAlno+XOd/NVrfL0WABrtGrtdTyX/x0e9Qy5bkGOLrCm/8yph+cmKE
GlYmNchZccUcGoUpEWr7QnJzVpna1rPkUQ3tf2C2MC4z
-----END CERTIFICATE-----`,
		},
	],
};

/**
 * Helper function to get a specific key by kid (Key ID)
 */
export function getKeyByKid(kid: string): JWK | undefined {
	return cloudflareAccessJWKS.keys.find((key) => key.kid === kid);
}

/**
 * Helper function to get a specific certificate by kid (Key ID)
 */
export function getCertificateByKid(kid: string): Certificate | undefined {
	return cloudflareAccessJWKS.public_certs.find((cert) => cert.kid === kid);
}

/**
 * Get all public keys as a JWKS object (for standard JWKS endpoints)
 */
export function getJWKS(): { keys: JWK[] } {
	return {
		keys: cloudflareAccessJWKS.keys,
	};
}
