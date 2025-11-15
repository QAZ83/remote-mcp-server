/**
 * AI Forge Studio - Website JavaScript
 * Interactive features and animations
 */

// ==========================================
// Smooth Scroll
// ==========================================
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        const target = document.querySelector(this.getAttribute('href'));
        if (target) {
            target.scrollIntoView({
                behavior: 'smooth',
                block: 'start'
            });
        }
    });
});

// ==========================================
// Mobile Menu Toggle
// ==========================================
const menuToggle = document.getElementById('menuToggle');
const navLinks = document.querySelector('.nav-links');

if (menuToggle) {
    menuToggle.addEventListener('click', () => {
        navLinks.classList.toggle('active');
        menuToggle.classList.toggle('active');
    });
}

// ==========================================
// Particles Animation
// ==========================================
function createParticles() {
    const particlesContainer = document.getElementById('particles');
    if (!particlesContainer) return;

    const particleCount = 50;

    for (let i = 0; i < particleCount; i++) {
        const particle = document.createElement('div');
        particle.className = 'particle';

        const size = Math.random() * 4 + 1;
        const x = Math.random() * 100;
        const y = Math.random() * 100;
        const duration = Math.random() * 20 + 10;
        const delay = Math.random() * 5;

        particle.style.cssText = `
            position: absolute;
            width: ${size}px;
            height: ${size}px;
            background: ${Math.random() > 0.5 ? '#00FFFF' : '#AA00FF'};
            border-radius: 50%;
            left: ${x}%;
            top: ${y}%;
            opacity: ${Math.random() * 0.5 + 0.2};
            animation: float ${duration}s ease-in-out ${delay}s infinite;
            box-shadow: 0 0 ${size * 2}px currentColor;
        `;

        particlesContainer.appendChild(particle);
    }
}

// CSS animation for particles
const style = document.createElement('style');
style.textContent = `
    @keyframes float {
        0%, 100% {
            transform: translate(0, 0) rotate(0deg);
        }
        25% {
            transform: translate(10px, -10px) rotate(90deg);
        }
        50% {
            transform: translate(-5px, -20px) rotate(180deg);
        }
        75% {
            transform: translate(-15px, -10px) rotate(270deg);
        }
    }
`;
document.head.appendChild(style);

// ==========================================
// Intersection Observer for Animations
// ==========================================
const observerOptions = {
    threshold: 0.1,
    rootMargin: '0px 0px -100px 0px'
};

const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.classList.add('fade-in');
            observer.unobserve(entry.target);
        }
    });
}, observerOptions);

// Observe all cards and sections
document.querySelectorAll('.feature-card, .stat-card, .tech-item, .download-card, .doc-card').forEach(el => {
    observer.observe(el);
});

// ==========================================
// Terminal Animation
// ==========================================
function animateTerminal() {
    const terminalLines = document.querySelectorAll('.terminal-line');
    if (terminalLines.length === 0) return;

    terminalLines.forEach((line, index) => {
        line.style.opacity = '0';
        line.style.transform = 'translateX(-10px)';

        setTimeout(() => {
            line.style.transition = 'all 0.3s ease';
            line.style.opacity = '1';
            line.style.transform = 'translateX(0)';
        }, index * 200);
    });
}

// ==========================================
// Stats Counter Animation
// ==========================================
function animateCounters() {
    const counters = document.querySelectorAll('.stat-number');

    counters.forEach(counter => {
        const target = counter.textContent;
        const isNumber = /^\d+$/.test(target);

        if (isNumber) {
            const updateCounter = () => {
                const targetNum = parseInt(target);
                const current = parseInt(counter.textContent);
                const increment = targetNum / 50;

                if (current < targetNum) {
                    counter.textContent = Math.ceil(current + increment);
                    setTimeout(updateCounter, 30);
                } else {
                    counter.textContent = target;
                }
            };

            updateCounter();
        }
    });
}

// ==========================================
// Header Scroll Effect
// ==========================================
let lastScroll = 0;
const header = document.querySelector('.header');

window.addEventListener('scroll', () => {
    const currentScroll = window.pageYOffset;

    if (currentScroll <= 0) {
        header.style.boxShadow = 'none';
    } else {
        header.style.boxShadow = '0 5px 20px rgba(0, 0, 0, 0.3)';
    }

    if (currentScroll > lastScroll && currentScroll > 100) {
        // Scrolling down
        header.style.transform = 'translateY(-100%)';
    } else {
        // Scrolling up
        header.style.transform = 'translateY(0)';
    }

    lastScroll = currentScroll;
});

// ==========================================
// Copy Code Functionality
// ==========================================
function addCopyButtons() {
    const codeBlocks = document.querySelectorAll('pre code');

    codeBlocks.forEach(codeBlock => {
        const button = document.createElement('button');
        button.className = 'copy-button';
        button.textContent = 'نسخ';

        button.addEventListener('click', () => {
            const code = codeBlock.textContent;
            navigator.clipboard.writeText(code).then(() => {
                button.textContent = 'تم النسخ!';
                setTimeout(() => {
                    button.textContent = 'نسخ';
                }, 2000);
            });
        });

        const pre = codeBlock.parentElement;
        pre.style.position = 'relative';
        pre.appendChild(button);
    });
}

// ==========================================
// Theme Toggle (Optional)
// ==========================================
function initThemeToggle() {
    const themeToggle = document.getElementById('themeToggle');
    if (!themeToggle) return;

    const currentTheme = localStorage.getItem('theme') || 'dark';
    document.documentElement.setAttribute('data-theme', currentTheme);

    themeToggle.addEventListener('click', () => {
        const theme = document.documentElement.getAttribute('data-theme');
        const newTheme = theme === 'dark' ? 'light' : 'dark';

        document.documentElement.setAttribute('data-theme', newTheme);
        localStorage.setItem('theme', newTheme);
    });
}

// ==========================================
// Download Stats (Optional)
// ==========================================
async function updateDownloadStats() {
    try {
        const response = await fetch('https://api.github.com/repos/QAZ83/remote-mcp-server/releases/latest');
        const data = await response.json();

        if (data.assets && data.assets.length > 0) {
            const totalDownloads = data.assets.reduce((sum, asset) => sum + asset.download_count, 0);

            const statsElements = document.querySelectorAll('.download-count');
            statsElements.forEach(el => {
                el.textContent = totalDownloads.toLocaleString('ar-EG');
            });
        }
    } catch (error) {
        console.log('Could not fetch download stats:', error);
    }
}

// ==========================================
// Form Validation (if needed)
// ==========================================
function validateForm(form) {
    const inputs = form.querySelectorAll('input[required], textarea[required]');
    let isValid = true;

    inputs.forEach(input => {
        if (!input.value.trim()) {
            isValid = false;
            input.classList.add('error');
        } else {
            input.classList.remove('error');
        }
    });

    return isValid;
}

// ==========================================
// Initialize Everything
// ==========================================
document.addEventListener('DOMContentLoaded', () => {
    // Create particles
    createParticles();

    // Animate terminal
    setTimeout(animateTerminal, 500);

    // Animate stats when visible
    const statsObserver = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                animateCounters();
                statsObserver.unobserve(entry.target);
            }
        });
    }, { threshold: 0.5 });

    const statsSection = document.querySelector('.stats');
    if (statsSection) {
        statsObserver.observe(statsSection);
    }

    // Add copy buttons to code blocks
    addCopyButtons();

    // Initialize theme toggle
    initThemeToggle();

    // Update download stats
    updateDownloadStats();

    console.log('%c🚀 AI Forge Studio Website Loaded', 'color: #00FFFF; font-size: 20px; font-weight: bold;');
    console.log('%cBuilt with ❤️ for RTX 50-Series', 'color: #AA00FF; font-size: 14px;');
});

// ==========================================
// Service Worker (for PWA - Optional)
// ==========================================
if ('serviceWorker' in navigator) {
    window.addEventListener('load', () => {
        navigator.serviceWorker.register('/sw.js')
            .then(registration => {
                console.log('ServiceWorker registered:', registration);
            })
            .catch(error => {
                console.log('ServiceWorker registration failed:', error);
            });
    });
}

// ==========================================
// Easter Egg: Konami Code
// ==========================================
const konamiCode = ['ArrowUp', 'ArrowUp', 'ArrowDown', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowLeft', 'ArrowRight', 'b', 'a'];
let konamiIndex = 0;

document.addEventListener('keydown', (e) => {
    if (e.key === konamiCode[konamiIndex]) {
        konamiIndex++;

        if (konamiIndex === konamiCode.length) {
            activateEasterEgg();
            konamiIndex = 0;
        }
    } else {
        konamiIndex = 0;
    }
});

function activateEasterEgg() {
    document.body.style.animation = 'rainbow 2s ease infinite';

    const style = document.createElement('style');
    style.textContent = `
        @keyframes rainbow {
            0% { filter: hue-rotate(0deg); }
            100% { filter: hue-rotate(360deg); }
        }
    `;
    document.head.appendChild(style);

    setTimeout(() => {
        document.body.style.animation = '';
    }, 5000);

    console.log('%c🎉 Easter Egg Activated!', 'color: #00FFFF; font-size: 30px; font-weight: bold;');
}
