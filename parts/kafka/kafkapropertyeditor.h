#ifndef _PROPERTY_EDITOR_H
#define _PROPERTY_EDITOR_H
#include <qlistview.h>
#include <qguardedptr.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qdict.h>

/*Temporarily - BEGIN*/
class KafkaPropertyEditorPluginBase: public QObject
{
	Q_OBJECT
public:
	KafkaPropertyEditorPluginBase(QObject *parent):QObject(parent){;}
	~KafkaPropertyEditorPluginBase(){;}
        virtual bool paintProperty ( const QString value, QPainter * p, const QColorGroup & cg, int column, int width,int height, int align ) {return false;}
	virtual QWidget *showInlineEditor(const QString &property, const QString &value, const QString &constrains, QWidget *parent)=0;
};

class KafkaPropertyEditor_Int: public KafkaPropertyEditorPluginBase
{
	Q_OBJECT
public:
	KafkaPropertyEditor_Int(QObject *parent):KafkaPropertyEditorPluginBase(parent){;}
	~KafkaPropertyEditor_Int(){;}
	virtual QWidget *showInlineEditor(const QString &property, const QString &value, const QString &constrains, QWidget *parent);
private:
	QString m_property;
protected slots:
	void slotValueChanged(int);
};



class KafkaPropertyEditor_TextLine: public KafkaPropertyEditorPluginBase
{
        Q_OBJECT
public:
        KafkaPropertyEditor_TextLine(QObject *parent):KafkaPropertyEditorPluginBase(parent){;}
        ~KafkaPropertyEditor_TextLine(){;}
        virtual QWidget *showInlineEditor(const QString &property, const QString &value, const QString &constrains, QWidget *parent);
private:
        QString m_property;
protected slots:
        void slotValueChanged(const QString&);
};


/*Temporarily - END*/


/*******************************************************************************
 *             An item implementation, which allows custom painted cells       *
 *******************************************************************************/


class KafkaPropertyEditorItem: public QListViewItem
{
public:
	KafkaPropertyEditorItem ( QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, 
		QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, 
		QString label7 = QString::null, QString label8 = QString::null );

	~KafkaPropertyEditorItem();

	virtual void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
};


/*********************************************************************************
 *  An listview implementation, which allows editing of items in different ways  *
 *********************************************************************************/

class KafkaPropertyEditor: public QListView
{
	Q_OBJECT
public:
	KafkaPropertyEditor(QWidget *parent);
	~KafkaPropertyEditor();
	void setProjectPath(const QString &projectPath);
	const QString& getProjectPath();
	void insertProperty(const QString &name, const QString &text, const QString &value,const QString &editor, const QString &constrains);
	QString getPropertyValue(const QString &property);
	//QStringList properties();
	void setValue(const QString &property, const QString &value,bool updateEditor=true);
	void setValue(const QString &value);
private:
	QString m_projectPath;
	QDict<KafkaPropertyEditorPluginBase> editors;
	void updateItem();
	QGuardedPtr<QWidget> currentInlineEditor;
	QListViewItem *oldItem;
protected:
	friend class KafkaPropertyEditorItem;
	bool paintCell ( QListViewItem *it, QPainter * p, const QColorGroup & cg, int column, int width,int height, int align );

protected slots:
	void columnSizeChange( int c, int os, int ns );
	void sectionClicked( int c );
	void slotSelectionChanged(QListViewItem*);

};

#endif
