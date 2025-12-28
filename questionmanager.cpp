#include "questionmanager.h"
#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>
#include "configmanager.h"

QuestionManager::QuestionManager(QObject *parent) : QObject(parent)
{
    // 默认使用循环模式
    questionMode = CycleMode;
    currentQuestionIndex = 0;

    // 连接配置变化信号
    connect(ConfigManager::getInstance(), &ConfigManager::configChanged,
            this, &QuestionManager::loadQuestionsAndKeywords);
}

QuestionManager::~QuestionManager()
{
}

void QuestionManager::loadQuestionsAndKeywords()
{
    // 从配置管理器加载问题列表和关键词
    ConfigManager* config = ConfigManager::getInstance();
    presetQuestions = config->getQuestionList();
    keywords = config->getKeywordList();

    emit logMessage(QString("已加载 %1 个预设问题和 %2 个关键词")
                        .arg(presetQuestions.size()).arg(keywords.size()));
}

QString QuestionManager::getNextQuestion()
{
    // 根据当前模式获取下一个问题
    switch (questionMode) {
    case CycleMode:
        return getNextPresetQuestion();
    case RandomMode:
        return getRandomPresetQuestion();
    case GenerateMode:
        // 随机决定是生成普通问题还是嵌入式相关问题
        if (QRandomGenerator::global()->bounded(100) < 30) { // 30%的概率生成嵌入式问题
            return generateEmbeddedQuestion();
        } else {
            return generateQuestion();
        }
    default:
        return getNextPresetQuestion();
    }
}

void QuestionManager::setQuestionMode(QuestionMode mode)
{
    questionMode = mode;

    QString modeStr;
    switch (mode) {
    case CycleMode: modeStr = "循环模式"; break;
    case RandomMode: modeStr = "随机模式"; break;
    case GenerateMode: modeStr = "生成模式"; break;
    }

    emit logMessage("问题模式已设置为: " + modeStr);
}

QuestionManager::QuestionMode QuestionManager::getQuestionMode() const
{
    return questionMode;
}

QString QuestionManager::getNextPresetQuestion()
{
    if (presetQuestions.isEmpty()) {
        emit logMessage("预设问题列表为空，尝试生成问题");
        return generateQuestion();
    }

    // 循环获取下一个问题
    QString question = presetQuestions[currentQuestionIndex];
    currentQuestionIndex = (currentQuestionIndex + 1) % presetQuestions.size();
    return question;
}

QString QuestionManager::getRandomPresetQuestion()
{
    if (presetQuestions.isEmpty()) {
        emit logMessage("预设问题列表为空，尝试生成问题");
        return generateQuestion();
    }

    // 随机获取一个问题
    int index = QRandomGenerator::global()->bounded(presetQuestions.size());
    return presetQuestions[index];
}

QString QuestionManager::generateQuestion()
{
    if (m_questionTemplates.isEmpty()) {
        emit logMessage("问题模板列表为空，无法生成问题");
        return "请问有什么可以帮助您？";
    }
    
    // 随机选择一个问题模板
    int templateIndex = QRandomGenerator::global()->bounded(m_questionTemplates.size());
    QString questionTemplate = m_questionTemplates[templateIndex];
    
    // 如果关键词列表为空，使用默认关键词
    if (keywords.isEmpty()) {
        emit logMessage("关键词列表为空，使用默认关键词");
        return questionTemplate.arg("微信小程序");
    }
    
    // 随机选择一个关键词
    int keywordIndex = QRandomGenerator::global()->bounded(keywords.size());
    QString keyword = keywords[keywordIndex];
    
    // 将关键词插入问题模板
    QString question = questionTemplate.arg(keyword);
    emit logMessage(QString("已生成问题：%1").arg(question));
    
    return question;
}

void QuestionManager::setPresetQuestions(const QStringList &list)
{
    presetQuestions = list.toVector();
}

void QuestionManager::setKeywords(const QStringList &list)
{
    keywords = list.toVector();
}

QVector<QString> QuestionManager::getPresetQuestions() const
{
    return presetQuestions;
}

QVector<QString> QuestionManager::getKeywords() const
{
    return keywords;
}

// 问题模板定义
const QStringList QuestionManager::m_questionTemplates = {
    // 基础问题模板
    "请回答：%1",
    "你觉得%1怎么样？",
    "请说出%1的特点。",
    
    // 微信小程序相关问题模板
    "如何使用%1开发微信小程序？",
    "微信小程序中%1的最佳实践是什么？",
    "请详细介绍微信小程序中的%1功能。",
    "微信小程序开发中%1有哪些注意事项？",
    "如何优化微信小程序中的%1性能？",
    "微信小程序%1与H5有什么区别？",
    "请解释微信小程序中%1的实现原理。",
    "微信小程序%1的应用场景有哪些？",
    "如何解决微信小程序开发中%1的常见问题？",
    "微信小程序%1的发展趋势是什么？",
    "请分析微信小程序%1的优缺点。",
    "微信小程序%1与原生App相比有什么优势？"
};

// 嵌入式问题模板定义
const QStringList QuestionManager::m_embeddedQuestionTemplates = {
    "请介绍嵌入式系统中%1的应用。",
    "嵌入式开发中%1有哪些特点？",
    "如何在嵌入式系统中实现%1功能？",
    "嵌入式设备中%1的优化方法有哪些？",
    "请解释嵌入式系统中%1的工作原理。",
    "嵌入式开发中如何解决%1问题？",
    "嵌入式系统中%1与普通系统有什么区别？",
    "在资源受限的嵌入式环境中，如何高效实现%1？",
    "嵌入式实时系统中%1的实现方法是什么？",
    "请分析嵌入式系统中%1的性能瓶颈。"
};

QString QuestionManager::generateEmbeddedQuestion()
{
    if (m_embeddedQuestionTemplates.isEmpty()) {
        emit logMessage("嵌入式问题模板列表为空，使用普通问题生成");
        return generateQuestion();
    }
    
    // 随机选择一个嵌入式问题模板
    int templateIndex = QRandomGenerator::global()->bounded(m_embeddedQuestionTemplates.size());
    QString questionTemplate = m_embeddedQuestionTemplates[templateIndex];
    
    // 如果关键词列表为空，使用默认关键词
    if (keywords.isEmpty()) {
        emit logMessage("关键词列表为空，使用默认关键词");
        return questionTemplate.arg("传感器");
    }
    
    // 随机选择一个关键词
    int keywordIndex = QRandomGenerator::global()->bounded(keywords.size());
    QString keyword = keywords[keywordIndex];
    
    // 将关键词插入问题模板
    QString question = questionTemplate.arg(keyword);
    emit logMessage(QString("已生成嵌入式问题：%1").arg(question));
    
    return question;
}
