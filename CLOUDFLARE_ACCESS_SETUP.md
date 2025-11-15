# Cloudflare Access JWT Validation Setup

This document explains how Cloudflare Access JWT validation has been implemented in this Worker to secure your MCP server.

## Overview

Cloudflare Access JWT validation ensures that only authenticated users from your Cloudflare Access can access your Worker application. All incoming requests are validated against Cloudflare Access's JSON Web Tokens (JWTs) before being processed.

## Configuration

### Environment Variables

The following environment variables are configured in `wrangler.jsonc`:

- **CLOUDFLARE_ACCESS_TEAM_DOMAIN**: `h-2883.cloudflareaccess.com`
- **CLOUDFLARE_ACCESS_POLICY_AUD**: `fe4b0e5cff2579c6d90b08238e70425b2875d15497b71fc4f1b6a90e49e7bef3`

These values are automatically loaded from your Cloudflare Access settings.

### How It Works

1. **Request Reception**: When a request arrives at your Worker, it includes a `CF-Access-JWT-Assertion` header added by Cloudflare Access.

2. **JWT Validation**: The middleware (`src/middleware.ts`) validates the JWT by:
   - Fetching the public keys from `https://h-2883.cloudflareaccess.com/cdn-cgi/access/certs`
   - Verifying the JWT signature using the Web Crypto API
   - Checking the token's expiration time
   - Validating the issuer matches your team domain
   - Ensuring the audience (aud) matches your policy AUD

3. **Access Control**: If validation succeeds, the request continues to your application. If it fails, a 401 Unauthorized response is returned.

## Implementation Details

### Files Modified

1. **`src/middleware.ts`** (NEW)
   - Contains the `cloudflareAccessMiddleware` function
   - Implements JWT validation using Web Crypto API
   - Caches JWKS keys for 1 hour to improve performance
   - Provides helper functions to access validated user data

2. **`src/app.ts`** (MODIFIED)
   - Added import for `cloudflareAccessMiddleware`
   - Applied middleware to all routes using `app.use('*', ...)`
   - Added environment variable types for TypeScript support

3. **`wrangler.jsonc`** (MODIFIED)
   - Added `vars` section with Cloudflare Access configuration

4. **`worker-configuration.d.ts`** (MODIFIED)
   - Updated `Env` interface to include new environment variables

### Security Features

- **Signature Verification**: Uses RSA-SHA256 cryptographic verification
- **Expiration Checking**: Rejects expired tokens automatically
- **Issuer Validation**: Ensures tokens come from your Cloudflare team
- **Audience Validation**: Verifies tokens are for your specific application
- **Key Caching**: Caches JWKS keys for performance while maintaining security

### Accessing User Information

In your route handlers, you can access the validated user information:

```typescript
import { getCloudflareAccessUser } from "./middleware";

app.get("/profile", async (c) => {
  const user = getCloudflareAccessUser(c);

  if (user) {
    return c.json({
      email: user.email,
      subject: user.sub,
      // other user claims...
    });
  }

  return c.json({ error: "Unauthorized" }, 401);
});
```

The user email is also stored in the context as `userEmail`:

```typescript
app.get("/profile", async (c) => {
  const email = c.get('userEmail');
  return c.json({ email });
});
```

## Testing

### Development Mode

When running locally with `wrangler dev`, Cloudflare Access validation can be disabled by not setting the environment variables. The middleware will automatically skip validation if the variables are not configured.

### Production

In production, ensure:
1. Cloudflare Access is enabled for your Worker domain: `*-aiforge-api.h-2883.workers.dev`
2. Access policies are configured in the Cloudflare dashboard
3. Only users with proper authentication can access your Worker

## Troubleshooting

### Common Errors

1. **"Missing CF-Access-JWT-Assertion header"**
   - Ensure Cloudflare Access is enabled for your domain
   - Check that requests are going through Cloudflare Access

2. **"Invalid JWT signature"**
   - Verify the `CLOUDFLARE_ACCESS_POLICY_AUD` matches your policy
   - Check that the team domain is correct

3. **"JWT has expired"**
   - The token has expired; user needs to re-authenticate
   - Check your Cloudflare Access session duration settings

4. **"Invalid issuer"**
   - Verify `CLOUDFLARE_ACCESS_TEAM_DOMAIN` is correct
   - Should be: `h-2883.cloudflareaccess.com`

## Security Notes

- **Never** disable JWT validation in production
- Keep your policy AUD value confidential
- Regularly review Cloudflare Access logs
- Monitor for authentication failures
- Only authorized Cloudflare account users can access this domain

## References

- [Cloudflare Access Documentation](https://developers.cloudflare.com/cloudflare-one/identity/authorization-cookie/validating-json/)
- [Cloudflare Workers Web Crypto API](https://developers.cloudflare.com/workers/runtime-apis/web-crypto/)
- [JWT Specification](https://datatracker.ietf.org/doc/html/rfc7519)

## Support

If you encounter issues with Cloudflare Access validation:
1. Check the Cloudflare Access logs in your dashboard
2. Review the Worker logs for validation errors
3. Verify your Access policies are correctly configured
4. Ensure your team domain and policy AUD are correct
