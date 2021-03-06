//
// Created by bentoo on 10/24/16.
//

#include "BasePage.h"
#include <QLayout>
#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>
#include <QtWidgets/QFrame>

namespace Editor
{
	BasePage::BasePage(QWidget *parent)
		: QWidget(parent)
	{
		QVBoxLayout* masterPageLayout = new QVBoxLayout();
		_toolbarLayout = new QHBoxLayout();

		masterPageLayout->setMargin(0);
		masterPageLayout->setSpacing(0);

		QFrame* toolbar = new QFrame();
		QFrame* content = new QFrame();

		toolbar->setObjectName("pageToolbar");
		content->setObjectName("pageContent");

		QLineEdit* search = new QLineEdit();
		search->setAccessibleDescription("Type to search...");

		addButton("nw","firstButton");
		addButton("op");
		addButton("sv","lastButton");

		_toolbarLayout->addSpacing(8);

		addButton("nxt","firstButton");
		addButton("prv","lastButton");

		_toolbarLayout->addSpacing(8);

		addButton("mv","firstButton");
		addButton("rt");
		addButton("sc","lastButton");

		_toolbarLayout->addStretch(1);
		_toolbarLayout->addWidget(search);
		addButton("sh", "lastButton");

		_toolbarLayout->addSpacing(8);

		addButton("hlp");

		_toolbarLayout->setSpacing(0);
		_toolbarLayout->setMargin(0);

		toolbar->setLayout(_toolbarLayout);
		toolbar->setFixedHeight(36);

		masterPageLayout->addWidget(toolbar);
		masterPageLayout->addWidget(content);

		this->setLayout(masterPageLayout);
		/*QToolBar* toolbar = new QToolBar(this);

		QWidget* segmentedButton = new QWidget(this);
		QToolButton* left = new QToolButton();
		QToolButton* right = new QToolButton();

		segmentedButton->setLayout(new QHBoxLayout());
		segmentedButton->layout()->setSpacing(0);
		segmentedButton->layout()->setMargin(0);

		int l = 0, r = 0, t = 0, b = 0;
		left->getContentsMargins(&l, &t, &r, &b);
		left->setContentsMargins(l - 4, t, r - 4, b);

		right->getContentsMargins(&l, &t, &r, &b);
		right->setContentsMargins(l - 4, t, r - 4, b);

		left->setFixedSize(28,28);
		right->setFixedSize(28,28);

		segmentedButton->layout()->addWidget(left);
		segmentedButton->layout()->addWidget(right);

		toolbar->addWidget(segmentedButton);*/
	}

	void BasePage::addButton(const char* text, const char* objName)
	{
		QToolButton* button = new QToolButton();
		button->setText(text);
		button->setObjectName(objName);
		_toolbarLayout->addWidget(button);
	}
}