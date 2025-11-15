import app from "./app";
import { McpAgent } from "agents/mcp";
import { McpServer } from "@modelcontextprotocol/sdk/server/mcp.js";
import { z } from "zod";
import OAuthProvider from "@cloudflare/workers-oauth-provider";

export class MyMCP extends McpAgent {
	server = new McpServer({
		name: "Calculator",
		version: "1.0.0",
	});

	async init() {
		// Addition
		this.server.tool("add", { a: z.number(), b: z.number() }, async ({ a, b }) => ({
			content: [{ type: "text", text: String(a + b) }],
		}));

		// Subtraction
		this.server.tool("subtract", { a: z.number(), b: z.number() }, async ({ a, b }) => ({
			content: [{ type: "text", text: String(a - b) }],
		}));

		// Multiplication
		this.server.tool("multiply", { a: z.number(), b: z.number() }, async ({ a, b }) => ({
			content: [{ type: "text", text: String(a * b) }],
		}));

		// Division
		this.server.tool("divide", { a: z.number(), b: z.number() }, async ({ a, b }) => {
			if (b === 0) {
				return {
					content: [{ type: "text", text: "Error: Division by zero is not allowed" }],
					isError: true,
				};
			}
			return {
				content: [{ type: "text", text: String(a / b) }],
			};
		});

		// Power (exponentiation)
		this.server.tool("power", { base: z.number(), exponent: z.number() }, async ({ base, exponent }) => ({
			content: [{ type: "text", text: String(Math.pow(base, exponent)) }],
		}));

		// Square root
		this.server.tool("sqrt", { value: z.number() }, async ({ value }) => {
			if (value < 0) {
				return {
					content: [{ type: "text", text: "Error: Cannot calculate square root of negative number" }],
					isError: true,
				};
			}
			return {
				content: [{ type: "text", text: String(Math.sqrt(value)) }],
			};
		});

		// Modulo
		this.server.tool("modulo", { a: z.number(), b: z.number() }, async ({ a, b }) => {
			if (b === 0) {
				return {
					content: [{ type: "text", text: "Error: Modulo by zero is not allowed" }],
					isError: true,
				};
			}
			return {
				content: [{ type: "text", text: String(a % b) }],
			};
		});
	}
}

// Export the OAuth handler as the default
export default new OAuthProvider({
	apiRoute: "/sse",
	// TODO: fix these types
	// @ts-expect-error
	apiHandler: MyMCP.mount("/sse"),
	// @ts-expect-error
	defaultHandler: app,
	authorizeEndpoint: "/authorize",
	tokenEndpoint: "/token",
	clientRegistrationEndpoint: "/register",
});
