#ifndef QUESTIONMANAGER_H
#define QUESTIONMANAGER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>

class QuestionManager : public QObject
{
    Q_OBJECT
public:
    // 问题模式
    enum QuestionMode {
        CycleMode,    // 循环模式
        RandomMode,   // 随机模式
        GenerateMode // 生成模式
    };
    Q_ENUM(QuestionMode)

    explicit QuestionManager(QObject *parent = nullptr);
    ~QuestionManager();

    // 加载预设问题列表
    bool loadPresetQuestions(const QString &filePath);

    // 保存预设问题列表
    bool savePresetQuestions(const QString &filePath);

    // 加载关键词库
    bool loadKeywordLibrary(const QString &filePath);

    // 保存关键词库
    bool saveKeywordLibrary(const QString &filePath);

    // 添加预设问题
    void addPresetQuestion(const QString &question);

    // 获取预设问题
    QVector<QString> getPresetQuestions() const;

    // 添加关键词
    void addKeyword(const QString &keyword);

    // 获取关键词库
    QVector<QString> getKeywords() const;

    // 随机获取一个预设问题
    QString getRandomPresetQuestion();

    // 自动生成一个嵌入式相关问题
    QString generateEmbeddedQuestion();

    // 在问题末尾添加提示
    QString addResponseLimitHint(const QString &question);

    // 设置问题模式
    void setQuestionMode(QuestionMode mode);
    
    // 获取问题模式
    QuestionMode getQuestionMode() const;

    // 获取下一个问题
    QString getNextQuestion();

    // 获取下一个预设问题
    QString getNextPresetQuestion();

    // 生成问题
    QString generateQuestion();

    void setPresetQuestions(const QStringList &list); // 新增声明
    void setKeywords(const QStringList &list);        // 新增声明

    // 智能问题生成
    QString generateSmartQuestion();
    
    // 基于上下文的问题生成
    QString generateContextualQuestion(const QString &previousQuestion);
    
    // 问题难度分级
    enum QuestionDifficulty {
        Easy,      // 简单问题
        Medium,    // 中等问题
        Hard       // 困难问题
    };
    
    // 根据难度生成问题
    QString generateQuestionByDifficulty(QuestionDifficulty difficulty);
    
    // 问题去重
    bool isQuestionUsed(const QString &question);
    
    // 问题质量评估
    double evaluateQuestionQuality(const QString &question);
    
    // 问题分类
    enum QuestionCategory {
        Basic,      // 基础概念
        Application, // 应用实践
        Advanced,   // 高级技术
        Troubleshooting // 故障排除
    };
    
    // 根据分类生成问题
    QString generateQuestionByCategory(QuestionCategory category);

signals:
    void logMessage(const QString &message);

private:
    QVector<QString> presetQuestions;
    QVector<QString> keywords;
    QVector<QString> questionPatterns; // 问题模式模板
    static const QStringList m_questionTemplates;
    static const QStringList m_embeddedQuestionTemplates; // 嵌入式问题模板

    QuestionMode questionMode;
    int currentQuestionIndex;

    // 初始化问题模式
    void initQuestionPatterns();

    // 加载问题和关键词
    void loadQuestionsAndKeywords();
};

#endif // QUESTIONMANAGER_H
