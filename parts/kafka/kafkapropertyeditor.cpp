#include "kafkapropertyeditor.h"
#include <klocale.h>
#include <qheader.h>
#include <qlineedit.h>

/* Temporary - BEGIN */
#include <knuminput.h>

QWidget *KafkaPropertyEditor_Int::showInlineEditor(const QString &property, const QString &value, const QString &constrains, QWidget *parent)
{
	KIntNumInput *ed=new KIntNumInput(parent);
	ed->setValue(value.toInt());
	m_property=property;
	connect(ed,SIGNAL(valueChanged(int)),this,SLOT(slotValueChanged(int)));
	return ed;
}

void KafkaPropertyEditor_Int::slotValueChanged(int val)
{
	((KafkaPropertyEditor*)parent())->setValue(QString("%1").arg(val));
}


QWidget *KafkaPropertyEditor_TextLine::showInlineEditor(const QString &property, const QString &value, const QString &constrains, QWidget *parent)
{
	QLineEdit *ed=new QLineEdit(parent);
	ed->setText(value);
	m_property=property;
	connect(ed,SIGNAL(textChanged(const QString&)),this,SLOT(slotValueChanged(const QString&)));
	return ed;
}

void KafkaPropertyEditor_TextLine::slotValueChanged(const QString &val)
{
	((KafkaPropertyEditor*)parent())->setValue(val);
}

/* Temporary - END */


KafkaPropertyEditorItem::KafkaPropertyEditorItem ( QListView * parent, QString label1, QString label2, QString label3,
                QString label4, QString label5, QString label6, QString label7, QString label8)
	:QListViewItem(parent,label1,label2,label3,label4,label5,label6,label7,label8)
{
}

KafkaPropertyEditorItem::~KafkaPropertyEditorItem(){;}

void KafkaPropertyEditorItem::paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align )
{
	if (column==1)
	{
		if (!(((KafkaPropertyEditor*)listView())->paintCell(this,p,cg,column,width,height(),align)))
			QListViewItem::paintCell(p,cg,column,width,align);
	}
	else QListViewItem::paintCell(p,cg,column,width,align);
}


//****************************************************************************************

KafkaPropertyEditor::KafkaPropertyEditor(QWidget *parent):QListView(parent)
{
	oldItem=0;
	addColumn( i18n( "Name" ) );
	addColumn( i18n( "Value" ) );
	setColumnWidthMode(1,QListView::Manual);
	header()->setMovingEnabled( FALSE );
	connect( header(), SIGNAL( sizeChange( int, int, int ) ),
		this, SLOT( columnSizeChange( int, int, int ) ) );
	disconnect( header(), SIGNAL( sectionClicked( int ) ),
		this, SLOT( changeSortColumn( int ) ) );
	connect(this,SIGNAL(selectionChanged(QListViewItem*)),this,SLOT(slotSelectionChanged(QListViewItem*)));
	editors.insert(QString("int"), new KafkaPropertyEditor_Int(this));
	editors.insert(QString("textline"), new KafkaPropertyEditor_TextLine(this));
}

KafkaPropertyEditor::~KafkaPropertyEditor()
{;}

void KafkaPropertyEditor::insertProperty(const QString &name, const QString &text, const QString &value,const QString &editor, const QString &constrains)
{
	insertItem(new QListViewItem(this,text,value,name,editor,constrains));
}

void KafkaPropertyEditor::setProjectPath(const QString &projectPath)
{
	m_projectPath=projectPath;
}

const QString& KafkaPropertyEditor::getProjectPath()
{
	return m_projectPath;
}


QString KafkaPropertyEditor::getPropertyValue(const QString &property)
{
	for (QListViewItem *it=firstChild();it;it=it->nextSibling())
        {
                qDebug(it->text(2)+"/"+property);
                if (it->text(2)==property) return it->text(1);
        }
}

void KafkaPropertyEditor::setValue(const QString &value)
{
	if (oldItem) oldItem->setText(1,value); else qDebug("no current item");
}

void KafkaPropertyEditor::setValue(const QString &property, const QString &value, bool updateEditor)
{
	for (QListViewItem *it=firstChild();it;it=it->nextSibling())
	{
		qDebug(it->text(2)+"/"+property);
		if (it->text(2)==property) {it->setText(1,value); /* if it==currentItem {}*/ break;}
		if (updateEditor)
		{
			if (it==oldItem) slotSelectionChanged(it);
		}
	}
}

void KafkaPropertyEditor::columnSizeChange( int c, int, int )
{
	if ( c == 0 || c == 1 )
		updateItem();
}

void KafkaPropertyEditor::sectionClicked( int )
{
	updateItem();
}

void KafkaPropertyEditor::slotSelectionChanged(QListViewItem* item)
{
	if (!item) return;
	oldItem=item;
	if (currentInlineEditor) delete currentInlineEditor;
	KafkaPropertyEditorPluginBase *tmp=editors[item->text(3)];
	if (tmp)
	{
		currentInlineEditor=tmp->showInlineEditor(item->text(2),item->text(1),item->text(4),viewport());
		currentInlineEditor->setFocus();
		addChild( currentInlineEditor );
		updateItem();
	}
}

void KafkaPropertyEditor::updateItem()
{
	if (!currentItem()) return;
	if (!currentInlineEditor) return;
	currentInlineEditor->resize( header()->cellSize( 1 ), currentItem()->height() );
	moveChild( currentInlineEditor, header()->cellPos( 1 ),
		itemPos( currentItem() ) + contentsY() );
	currentInlineEditor->show();

}


bool KafkaPropertyEditor::paintCell ( QListViewItem *it, QPainter * p, const QColorGroup & cg, int column, int width,int height, int align )
	{
		if (!it) return false;
		KafkaPropertyEditorPluginBase *plugin=editors[it->text(3)];
		if (!plugin) return false;
		return plugin->paintProperty ( it->text(1), p ,  cg,  column, width, height, align );
	}
#include "kafkapropertyeditor.moc"
