// -*- c++ -*-

#ifndef QUANTA_ACTIONS_H
#define QUANTA_ACTIONS_H

#include <kaction.h>
#include <qstring.h>

class QuantaView;

/**
 * An action for inserting an XML tag.
 *
 * @author Richard Moore, rich@kde.org
 * @version $Id$
 */
class QuantaTagAction : public KAction
{
    Q_OBJECT

    Q_PROPERTY( QString openTag READ openTag WRITE setOpenTag DESIGNABLE true )
    Q_PROPERTY( QString closeTag READ closeTag WRITE setCloseTag DESIGNABLE true )
    Q_PROPERTY( bool allOnOneLine READ allOnOneLine WRITE setAllOnOneLine DESIGNABLE true )
    Q_PROPERTY( bool showTagDialog READ showTagDialog WRITE setShowTagDialog DESIGNABLE true )

public:
    /**
     * Create an insert without an icon.
     */
    QuantaTagAction( const QString& text, int accel,
                     QObject* parent, const char* name, QuantaView *view );

    /**
     * Create an insert with an icon.
     */
    QuantaTagAction( const QString& text, const QString& pix, int accel,
                     QObject* parent, const char* name, QuantaView *view );

    virtual ~QuantaTagAction();

    QString openTag() const { return openTag_; }
    void setOpenTag( const QString &open ) { openTag_ = open; }

    QString closeTag() const { return closeTag_; }
    void setCloseTag( const QString &close ) { closeTag_ = close; }

    bool allOnOneLine() const { return allOnOneLine_; }
    void setAllOnOneLine( bool enable ) { allOnOneLine_ = enable; }

    bool showTagDialog() const { return showTagDialog_; }
    void setShowTagDialog( bool show ) { showTagDialog_ = show; }

    void setTag( const QString &openTag, const QString &closeTag = QString::null );

protected slots:
    virtual void insertTag();

private:
    QString openTag_;
    QString closeTag_;
    bool allOnOneLine_;
    bool showTagDialog_;
    QuantaView *view_;
};

/**
 * An action for running an external filter script.
 *
 * @author Richard Moore, rich@kde.org
 */
class QuantaScriptAction : public KAction
{
    Q_OBJECT

    Q_PROPERTY( QString script READ script WRITE setScript DESIGNABLE true )
    Q_PROPERTY( ScriptInput input READ input WRITE setInput DESIGNABLE true )
    Q_PROPERTY( ScriptOutput output READ output WRITE setOutput DESIGNABLE true )
    Q_ENUMS( ScriptInput )
    Q_ENUMS( ScriptOutput )

public:
    /**
     * What we send to the filter.
     */
    enum ScriptInput {
        NoInput,
        Selection,
        Document
    };

    /**
     * What the sends us.
     */
    enum ScriptOutput {
        NoOutput,
        InsertAtCursor,
        ReplaceDocument,
        NewDocument,
        MessageWindow,
        MergeWithOutput
    };

    /**
     * Create an insert without an icon.
     */
    QuantaScriptAction( const QString& text, int accel,
                        const QObject* receiver, const char* slot,
                        QObject* parent, const char* name = 0 );

    /**
     * Create an insert with an icon.
     */
    QuantaScriptAction( const QString& text, const QString& pix, int accel,
                        const QObject* receiver, const char* slot,
                        QObject* parent, const char* name );

    virtual ~QuantaScriptAction();

    QString script() const { return filename_; }
    void setScript( const QString &file ) { filename_ = file; }

    ScriptInput input() const { return input_; }
    void setInput( const ScriptInput in ) { this->input_ = in; }

    ScriptOutput output() const { return output_; }
    void setOutput( const ScriptOutput out ) { this->output_ = out; }

protected slots:
    virtual void runScript();

private:
    QString filename_;
    ScriptInput input_;
    ScriptOutput output_;
};

#endif // QUANTA_ACTIONS_H
