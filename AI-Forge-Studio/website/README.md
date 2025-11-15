# AI Forge Studio Website

Official website for AI Forge Studio - RTX 50-Series Edition

🌐 **Live Site:** [aiforgestudio.net](https://aiforgestudio.net)

## 📁 Structure

```
website/
├── index.html              # Main landing page
├── css/
│   └── style.css          # Glass-morphism styles
├── js/
│   └── main.js            # Interactive features
├── docs/
│   ├── getting-started.html
│   ├── user-guide.html
│   ├── api-reference.html
│   └── troubleshooting.html
├── images/                 # Logo and screenshots
├── CNAME                   # Custom domain config
├── robots.txt              # SEO crawling rules
└── sitemap.xml             # SEO sitemap

```

## 🎨 Design

- **Theme:** Glass-morphism with Dark Mode
- **Colors:** Neon Cyan (#00FFFF) & Purple (#AA00FF)
- **Fonts:** Tajawal (Arabic), Roboto Mono (Code)
- **Framework:** Pure HTML/CSS/JS (No dependencies)

## 🚀 Deployment

### GitHub Pages

1. Push to `main` branch
2. Go to Settings → Pages
3. Set source to `website/` folder
4. Add custom domain: `aiforgestudio.net`

### Custom Domain (Cloudflare)

1. Add CNAME record:
   ```
   aiforgestudio.net → QAZ83.github.io
   www.aiforgestudio.net → QAZ83.github.io
   ```

2. Enable SSL/TLS encryption (Full)

3. Configure Page Rules for redirects

## 📝 SEO

- Semantic HTML5
- Meta tags (Open Graph, Twitter Cards)
- Sitemap.xml
- Robots.txt
- Fast loading (<2s)
- Mobile responsive

## 🛠️ Development

### Local Testing

```bash
# Simple HTTP server
python3 -m http.server 8000

# OR
npx serve website/
```

Visit: `http://localhost:8000`

### Build Optimization

- Minify CSS/JS for production
- Optimize images (WebP format)
- Enable compression (Gzip/Brotli)
- Configure CDN caching

## 📊 Analytics

Recommended tools:
- Google Analytics
- Cloudflare Analytics
- GitHub traffic insights

## 🔒 Security

- HTTPS enforced
- CSP headers
- No external dependencies
- Static site (no backend)

## 📧 Contact

- Website: [aiforgestudio.net](https://aiforgestudio.net)
- GitHub: [QAZ83/remote-mcp-server](https://github.com/QAZ83/remote-mcp-server)
- Email: info@aiforgestudio.net

---

Built with ❤️ for RTX 50-Series users
