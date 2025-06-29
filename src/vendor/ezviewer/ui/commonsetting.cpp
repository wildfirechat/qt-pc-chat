/****************************************************************************
 * EZ Viewer
 * Copyright (C) 2012 huangezhao. CHINA.
 * Contact: huangezhao (huangezhao@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#include "commonsetting.h"
#include "ui_commonsetting.h"

#include "imageviewer_config.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QButtonGroup>


CommonSetting::CommonSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonSetting)
{
    ui->setupUi(this);
    ui->ui_groupBox->hide();

    showDialogCheckBox = ui->showDialogCheckBox;
    scaleModeCombo = ui->scaleModeCombo;
    antialiasModeCombo = ui->antialiasModeCombo;
    timerSpinBox = ui->timerSpinBox;
    colorButton = ui->colorButton;
    colorEdit = ui->colorEdit;
    colorCheckBox = ui->colorCheckBox;
    colorLabel = ui->colorLabel;
    autoRotateCheckBox = ui->autoRotateCheckBox;
    preReadingCheckBox = ui->preReadingCheckBox;
    cacheValueLabel = ui->cacheValueLabel;
    cacheValueSlider = ui->cacheValueSlider;

    alignButtonGroup = new QButtonGroup(this);
    alignButtonGroup->addButton(ui->leftTopButton, IVConfig::AlignLeftTop);
    alignButtonGroup->addButton(ui->centerTopButton, IVConfig::AlignCenterTop);
    alignButtonGroup->addButton(ui->rightTopButton, IVConfig::AlignRightTop);
    alignButtonGroup->addButton(ui->leftCenterButton, IVConfig::AlignLeftCenter);
    alignButtonGroup->addButton(ui->centerCenterButton, IVConfig::AlignCenterCenter);
    alignButtonGroup->addButton(ui->rightCenterButton, IVConfig::AlignRightCenter);
    alignButtonGroup->addButton(ui->leftBottomButton, IVConfig::AlignLeftBottom);
    alignButtonGroup->addButton(ui->centerBottomButton, IVConfig::AlignCenterBottom);
    alignButtonGroup->addButton(ui->rightBottomButton, IVConfig::AlignRightBottom);

    initUIvalue();

    connect(showDialogCheckBox, SIGNAL(stateChanged(int)),
            SLOT(showDialogChange(int)));
    connect(scaleModeCombo, SIGNAL(currentIndexChanged(int)),
            SLOT(scaleModeChange(int)));
    connect(alignButtonGroup, SIGNAL(buttonClicked(int)),
            SLOT(alignModeChange(int)));
    connect(antialiasModeCombo, SIGNAL(currentIndexChanged(int)),
            SLOT(antialiasModeChange(int)));
    connect(colorButton, SIGNAL(clicked()), SLOT(setColor()));
    connect(colorCheckBox, SIGNAL(stateChanged(int)),
            SLOT(bgColorEnable(int)));
    connect(timerSpinBox, SIGNAL(valueChanged(int)),
            SLOT(timerIntervalChange(int)));
    connect(autoRotateCheckBox, SIGNAL(stateChanged(int)),
            SLOT(autoRotateImageChanged(int)));
    connect(preReadingCheckBox, SIGNAL(stateChanged(int)),
            SLOT(preReadingChanged(int)));
    connect(cacheValueSlider, SIGNAL(valueChanged(int)),
            SLOT(cacheValueChanged(int)));

    QDialogButtonBox *buttonBox = ui->buttonBox;
    QPushButton *button = buttonBox->addButton(QDialogButtonBox::Close);
    button->setDefault(true);
    connect(button, SIGNAL(clicked()), SIGNAL(clickClose()));
    button = buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
    connect(button, SIGNAL(clicked()), SLOT(restoreDefaults()));

    IVConfig::insertConfigWatcher(this, SLOT(initUIvalue()));
}

CommonSetting::~CommonSetting()
{
    IVConfig::cancelConfigWatcher(this);
    delete ui;
}

void CommonSetting::initUIvalue()
{
    showDialogCheckBox->setChecked(IVConfig::showDialog());
    scaleModeCombo->setCurrentIndex(IVConfig::scaleMode());
    alignButtonGroup->button(IVConfig::alignMode())->setChecked(true);
    antialiasModeCombo->setCurrentIndex(IVConfig::antialiasMode());
    timerSpinBox->setValue(IVConfig::timerInterval());
    autoRotateCheckBox->setChecked(IVConfig::autoRotateImage());
    preReadingCheckBox->setChecked(IVConfig::enablePreReading());
    cacheValueLabel->setText(QString::number(IVConfig::cacheNum()));
    cacheValueSlider->setValue(IVConfig::cacheNum());

    changeColorSettingUI(IVConfig::bgColor());
    bool enabledBgColor = IVConfig::enableBgColor();
    colorCheckBox->setChecked(enabledBgColor);
    colorLabel->setEnabled(enabledBgColor);
    colorButton->setEnabled(enabledBgColor);
    colorEdit->setEnabled(enabledBgColor);
}

void CommonSetting::changeColorSettingUI(const QColor &color)
{
    QPixmap pix(25, 25);
    pix.fill(color);
    colorButton->setIcon(QIcon(pix));
    colorEdit->setText(color.name());
}

void CommonSetting::showDialogChange(int state)
{
    IVConfig::setShowDialog(state == Qt::Checked);
}

void CommonSetting::scaleModeChange(int index)
{
    if(index == -1) return;
    IVConfig::setScaleMode(IVConfig::ScaleMode(index));
}

void CommonSetting::alignModeChange(int id)
{
    if (id < IVConfig::AlignModeBegin || id > IVConfig::AlignModeEnd) return;
    IVConfig::setAlignMode(IVConfig::AlignMode(id));
}

void CommonSetting::antialiasModeChange(int index)
{
    if(index == -1) return;
    IVConfig::setAntialiasMode(IVConfig::AntialiasMode(index));
}

void CommonSetting::bgColorEnable(int state)
{
    bool enableBgColor = (state == Qt::Checked);
    IVConfig::setEnableBgColor(enableBgColor);

    colorLabel->setEnabled(enableBgColor);
    colorButton->setEnabled(enableBgColor);
    colorEdit->setEnabled(enableBgColor);
}

void CommonSetting::setColor()
{
    QColor color = QColorDialog::getColor(IVConfig::bgColor(), this);
    if (color.isValid()) {
        changeColorSettingUI(color);
        IVConfig::setBgColor(color);
    }
}

void CommonSetting::timerIntervalChange(int val)
{
    IVConfig::setTimerInterval(val);
}

void CommonSetting::autoRotateImageChanged(int state)
{
    IVConfig::setAutoRotateImage(state == Qt::Checked);
}

void CommonSetting::preReadingChanged(int state)
{
    IVConfig::setEnablePreReading(state == Qt::Checked);
}

void CommonSetting::cacheValueChanged(int val)
{
    IVConfig::setCacheValue(val);
    cacheValueLabel->setText(QString::number(val));
}

void CommonSetting::restoreDefaults()
{
    IVConfig::restoreDefaultsConfig();
}

