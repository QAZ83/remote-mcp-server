# Cloudflare Access RSA Certificates

This document explains the RSA certificates and keys configured for Cloudflare Access authentication.

## Overview

The RSA public keys and certificates in `src/cloudflare-access-jwks.ts` are used for verifying JWT tokens issued by Cloudflare Access. These keys follow the JWKS (JSON Web Key Set) standard format.

## Certificate Details

### Keys Available

The configuration includes two RSA public keys:

1. **Key ID**: `fca035a37fed690288402e5e0ce8fbacd846fb011b393bb497c0887c077b0f06`
   - Algorithm: RS256
   - Usage: Signature verification
   - Valid: 2025-11-15 to 2026-11-29

2. **Key ID**: `59659d34818532fdc3383571f513b7717b6277497124671f57940c45213dfbc1`
   - Algorithm: RS256
   - Usage: Signature verification
   - Valid: 2025-11-15 to 2026-11-29

### Certificate Information

- **Issuer**: Cloudflare (cloudflareaccess.com)
- **Location**: Austin, Texas, US
- **Algorithm**: RSA with SHA-256
- **Key Size**: 2048 bits

## Usage

### Importing the JWKS Configuration

```typescript
import { cloudflareAccessJWKS, getKeyByKid, getCertificateByKid, getJWKS } from './cloudflare-access-jwks';
```

### Getting a Specific Key

```typescript
// Get a key by its Key ID
const key = getKeyByKid('59659d34818532fdc3383571f513b7717b6277497124671f57940c45213dfbc1');
```

### Getting a Specific Certificate

```typescript
// Get a certificate by its Key ID
const cert = getCertificateByKid('59659d34818532fdc3383571f513b7717b6277497124671f57940c45213dfbc1');
```

### Exposing JWKS Endpoint

You can expose a JWKS endpoint for token verification:

```typescript
import { getJWKS } from './cloudflare-access-jwks';

app.get('/jwks.json', (c) => {
  return c.json(getJWKS());
});
```

## Integration with OAuth

These certificates are used to verify JWT tokens in the OAuth flow:

1. Client requests authorization
2. Cloudflare Access issues a JWT token signed with the private key
3. This server verifies the token using the public keys from the JWKS
4. If verification succeeds, the user is authenticated

## Security Considerations

- These are **public keys** - they can be safely committed to version control
- The corresponding private keys are securely held by Cloudflare
- Keys should be rotated periodically (current expiry: 2026-11-29)
- Always verify the `kid` (Key ID) in the JWT header matches an available key

## Verifying JWT Tokens

To verify a JWT token from Cloudflare Access:

1. Extract the `kid` from the JWT header
2. Fetch the corresponding public key using `getKeyByKid(kid)`
3. Verify the token signature using the public key
4. Validate the token claims (expiry, audience, issuer, etc.)

## Key Rotation

When keys are rotated:

1. Update the `cloudflareAccessJWKS` object in `src/cloudflare-access-jwks.ts`
2. Keep old keys for a grace period to allow in-flight tokens to be verified
3. Remove expired keys after the grace period

## References

- [Cloudflare Access Documentation](https://developers.cloudflare.com/cloudflare-one/identity/authorization-cookie/)
- [JWKS Specification](https://tools.ietf.org/html/rfc7517)
- [JWT Best Practices](https://tools.ietf.org/html/rfc8725)
