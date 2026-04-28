# 本地智能对话机器人项目 🤖

## 项目目标

在本地运行一个智能对话助手，不依赖云端 API，数据完全私有。
可以作为：
- 充电桩运维知识问答助手
- 故障排查辅助工具
- 嵌入式开发学习伙伴

## 技术方案

### 方案A：Ollama + 开源模型（推荐入门）

最简单的方式，5分钟上手！

```bash
# 1. 安装 Ollama（macOS/Linux）
curl -fsSL https://ollama.ai/install.sh | sh

# 2. 下载模型（约4GB，需要好一点的网络）
ollama pull llama3.2:3b    # 3B参数，普通电脑可运行

# 3. 对话
ollama run llama3.2:3b
```

### 方案B：Python API 封装

```python
import ollama

response = ollama.chat(
    model='llama3.2:3b',
    messages=[
        {'role': 'system', 'content': '你是一个充电桩运维专家，用中文回答问题。'},
        {'role': 'user',   'content': '充电桩出现E001故障码是什么原因？'}
    ]
)
print(response['message']['content'])
```

## 文件清单

| 文件 | 说明 | 状态 |
|------|------|------|
| `chatbot_basic.py` | 基础对话机器人 | TODO：待创建 |
| `rag_charger.py` | 基于充电桩文档的问答（RAG）| TODO：待创建 |
| `fault_diagnosis.py` | 故障诊断助手 | TODO：待创建 |

## TODO 练习

- [ ] TODO-01：安装 Ollama，成功运行一个对话
- [ ] TODO-02：用 Python 封装对话 API，实现多轮对话
- [ ] TODO-03：给机器人添加"系统提示词"（System Prompt），让它扮演充电桩专家
- [ ] TODO-04：实现 RAG（检索增强生成）：把充电桩手册喂给模型
  - 将文档切片存入向量数据库（chromadb）
  - 用户问题先检索相关文档片段
  - 再结合文档片段让 LLM 回答
- [ ] TODO-05：实现对话历史记录（保存到 JSON 文件）
- [ ] TODO-06：添加故障码解释功能（E001, E002... → 解释 + 处理建议）
- [ ] TODO-07：用语音输入/输出（whisper + pyttsx3）实现语音交互

## 所需依赖

```bash
pip install ollama langchain chromadb sentence-transformers
```

## 充电桩场景应用构想

**运维知识问答**：
- "E003 故障码怎么处理？"
- "交流充电桩过温保护阈值一般是多少？"
- "IEC 61851 标准中 CP 信号的各状态是什么？"

**代码辅助**：
- "帮我写一个 GD32 的 UART 初始化代码"
- "这段 Modbus CRC 计算有什么问题？"
