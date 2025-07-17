# Security Policy | 安全政策

[English](#english) | [中文](#中文)

## English

## 🔒 Supported Versions

We actively support the following versions of OpenCap with security updates:

| Version | Supported          | Notes                    |
| ------- | ------------------ | ------------------------ |
| 1.0.x   | ✅ Yes             | Current stable release   |
| < 1.0   | ❌ No              | Pre-release versions     |

## 🚨 Reporting a Vulnerability

We take security vulnerabilities seriously. If you discover a security vulnerability in OpenCap, please follow the responsible disclosure process outlined below.

### 📧 How to Report

**Please DO NOT report security vulnerabilities through public GitHub issues.**

Instead, please send a detailed report to our security team via email:

**Email**: [security@your-domain.com](mailto:security@your-domain.com)
<!-- Replace with your actual security contact email -->

### 📋 What to Include

When reporting a vulnerability, please include the following information:

- **Type of vulnerability** (e.g., buffer overflow, SQL injection, cross-site scripting, etc.)
- **Full paths and names of the source files** related to the vulnerability
- **Location of the affected source code** (tag/branch/commit or direct URL)
- **Step-by-step instructions** to reproduce the issue
- **Proof-of-concept or exploit code** (if available)
- **Impact assessment** - what an attacker could potentially do
- **Your assessment of the severity** of the vulnerability

### 🕒 Response Timeline

We commit to the following response timeline:

- **Initial Response**: Within 48 hours of receiving your report
- **Investigation**: Within 7 days, we will begin investigation
- **Status Updates**: We will provide updates every 7 days during investigation
- **Resolution**: We aim to resolve critical vulnerabilities within 30 days

### 🎯 Vulnerability Scope

#### In Scope
Security vulnerabilities in the following areas are within scope:

- **Screenshot capture mechanism** - Unauthorized screen access
- **File system operations** - Path traversal, arbitrary file access
- **Memory safety** - Buffer overflows, use-after-free
- **Privilege escalation** - Unauthorized system access
- **Input validation** - Malformed data handling
- **macOS system integration** - Cocoa API misuse
- **Build system security** - Supply chain attacks

#### Out of Scope
The following are generally considered out of scope:

- **Physical access attacks** - Requiring physical access to the device
- **Social engineering attacks** - Targeting users rather than the application
- **Third-party dependencies** - Vulnerabilities in Qt or system libraries (report to upstream)
- **Denial of Service** - Unless it leads to code execution or data exposure
- **UI/UX issues** - That don't have security implications

### 🏆 Recognition

We appreciate security researchers who help us maintain the security of OpenCap:

- **Acknowledgment**: We will acknowledge your contribution in our security advisories (with your permission)
- **Hall of Fame**: Security researchers will be recognized in our project documentation
- **Communication**: We will keep you informed throughout the resolution process

### 🔐 Security Measures

#### Current Security Practices

- **Code Review**: All code changes are reviewed before merging
- **Static Analysis**: Automated security scanning in our CI/CD pipeline
- **Memory Safety**: Use of modern C++ practices and RAII
- **Minimal Privileges**: Application requests only necessary system permissions
- **Secure Defaults**: Conservative default settings and configurations

#### macOS Security Integration

- **System Permissions**: Proper handling of screen recording permissions
- **Sandboxing**: Consideration for App Store sandboxing requirements
- **Code Signing**: Plans for proper code signing in releases
- **Notarization**: Future support for macOS notarization

### 📜 Security Advisories

Security advisories will be published at:

- **GitHub Security Advisories**: [GitHub Security Tab](https://github.com/yourusername/openCap/security/advisories)
- **Release Notes**: Security fixes will be documented in release notes
- **CHANGELOG.md**: Security-related changes will be noted in our changelog

### 🔄 Security Updates

When security vulnerabilities are fixed:

1. **Patch Development**: We develop and test the security fix
2. **Coordinated Disclosure**: We coordinate with the reporter on disclosure timing
3. **Release**: We release a patched version as soon as possible
4. **Advisory**: We publish a security advisory with details
5. **Communication**: We notify users through our communication channels

### 📞 Contact Information

For security-related inquiries:

- **Security Email**: security@your-domain.com
- **GPG Key**: [Link to GPG public key if available]
- **Response Time**: We aim to respond within 48 hours

For general questions about this security policy:

- **GitHub Issues**: For non-security related questions about this policy
- **Discussions**: Use GitHub Discussions for general security questions

### 🙏 Thank You

We appreciate the security research community's efforts to help keep OpenCap and our users safe. Responsible disclosure helps us protect our users while giving us the opportunity to fix vulnerabilities before they can be exploited.

---

**Last Updated**: January 2024
**Policy Version**: 1.0

---

## 中文

## 🔒 支持的版本

我们为以下版本的OpenCap提供安全更新支持：

| 版本    | 支持状态          | 说明                    |
| ------- | ------------------ | ------------------------ |
| 1.0.x   | ✅ 是             | 当前稳定版本   |
| < 1.0   | ❌ 否              | 预发布版本     |

## 🚨 报告漏洞

我们认真对待安全漏洞。如果您发现OpenCap中的安全漏洞，请遵循下面概述的负责任披露流程。

### 📧 如何报告

**请勿通过公共GitHub问题报告安全漏洞。**

相反，请通过电子邮件向我们的安全团队发送详细报告：

**邮箱**: [security@your-domain.com](mailto:security@your-domain.com)
<!-- 替换为您的实际安全联系邮箱 -->

### 📋 包含的信息

报告漏洞时，请包含以下信息：

- **漏洞类型**（例如，缓冲区溢出、SQL注入、跨站脚本等）
- **与漏洞相关的源文件的完整路径和名称**
- **受影响源代码的位置**（标签/分支/提交或直接URL）
- **重现问题的分步说明**
- **概念验证或漏洞利用代码**（如果可用）
- **影响评估** - 攻击者可能做什么
- **您对漏洞严重性的评估**

### 🕒 响应时间表

我们承诺以下响应时间表：

- **初始响应**：收到报告后48小时内
- **调查**：7天内开始调查
- **状态更新**：调查期间每7天提供更新
- **解决**：我们的目标是在30天内解决关键漏洞

### 🎯 漏洞范围

#### 范围内
以下领域的安全漏洞在范围内：

- **截图捕获机制** - 未授权的屏幕访问
- **文件系统操作** - 路径遍历、任意文件访问
- **内存安全** - 缓冲区溢出、释放后使用
- **权限提升** - 未授权的系统访问
- **输入验证** - 恶意数据处理
- **macOS系统集成** - Cocoa API误用
- **构建系统安全** - 供应链攻击

#### 范围外
以下通常被认为在范围外：

- **物理访问攻击** - 需要对设备的物理访问
- **社会工程攻击** - 针对用户而非应用程序
- **第三方依赖** - Qt或系统库中的漏洞（请报告给上游）
- **拒绝服务** - 除非导致代码执行或数据暴露
- **UI/UX问题** - 没有安全影响的问题

### 🏆 认可

我们感谢帮助我们维护OpenCap安全的安全研究人员：

- **致谢**：我们将在安全公告中致谢您的贡献（经您同意）
- **名人堂**：安全研究人员将在我们的项目文档中得到认可
- **沟通**：我们将在整个解决过程中让您了解情况

### 🔐 安全措施

#### 当前安全实践

- **代码审查**：所有代码更改在合并前都经过审查
- **静态分析**：我们的CI/CD流水线中的自动安全扫描
- **内存安全**：使用现代C++实践和RAII
- **最小权限**：应用程序只请求必要的系统权限
- **安全默认值**：保守的默认设置和配置

#### macOS安全集成

- **系统权限**：正确处理屏幕录制权限
- **沙盒化**：考虑App Store沙盒要求
- **代码签名**：计划在发布中进行适当的代码签名
- **公证**：未来支持macOS公证

### 📜 安全公告

安全公告将发布在：

- **GitHub安全公告**：[GitHub安全选项卡](https://github.com/yourusername/openCap/security/advisories)
- **发布说明**：安全修复将在发布说明中记录
- **CHANGELOG.md**：安全相关更改将在我们的更新日志中注明

### 🔄 安全更新

当安全漏洞被修复时：

1. **补丁开发**：我们开发和测试安全修复
2. **协调披露**：我们与报告者协调披露时间
3. **发布**：我们尽快发布修补版本
4. **公告**：我们发布带有详细信息的安全公告
5. **沟通**：我们通过我们的沟通渠道通知用户

### 📞 联系信息

对于安全相关查询：

- **安全邮箱**：security@your-domain.com
- **GPG密钥**：[GPG公钥链接（如果可用）]
- **响应时间**：我们的目标是在48小时内响应

对于有关此安全政策的一般问题：

- **GitHub Issues**：对于关于此政策的非安全相关问题
- **讨论**：使用GitHub讨论进行一般安全问题

### 🙏 致谢

我们感谢安全研究社区帮助保持OpenCap和我们用户的安全。负责任的披露帮助我们保护用户，同时给我们修复漏洞的机会。

---

**最后更新**：2024年1月
**政策版本**：1.0 