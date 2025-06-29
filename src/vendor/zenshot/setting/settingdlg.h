#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>

#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include "ui_setting.h"

class SettingDlg : public QDialog {
    Q_OBJECT

    enum {
        BTN_ID_ZENTAO = 0,
        BTN_ID_SHORTKEY,
    };
public:
    explicit SettingDlg(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~SettingDlg();

protected:
	void closeEvent(QCloseEvent*) override;
	void showEvent(QShowEvent*) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
#else
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);
#endif

private:
    void initDlg();
    void initStat();
    void setupSingal();
    QString getHotKeyStr(uint32_t value);

signals:
    void SaveHotKeyConfig();
    void UpdateHotKeyText(uint32_t value);
    void UpdateHotKeyResult(bool success);
    void InitHotKeyValue(uint32_t value);

private slots:
    void OnUpdateHotKeyText(uint32_t value);                    // �����ȼ���ʾ
    void OnUpdateHotKeyValue();                                 // ע���ȼ�
    void OnUpdateHotKeyResult(bool success);                    // �ȼ�ע����
    void OnInitHotKeyValue(uint32_t value);                     // ��ʼ���ȼ�
    void OnSaveHotKeyConfig();

private:
    Ui::SettingDlg ui;
    uint32_t m_KeyValue, m_OrigKeyValue;
    uint32_t m_SetKeyValue[3];
    QPushButton* m_HotKeyStat;
    QPushButton* m_HotKeyValue;
}; // SettingDlg

#endif // SETTINGDLG_H
