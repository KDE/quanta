// -*- c++ -*-

#ifndef QUANTA_ACTIONS_H
#define QUANTA_ACTIONS_H

#include <kaction.h>
#include <qstring.h>

class QuantaView;
class KProcess;

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
    Q_PROPERTY( ScriptOutput error READ error WRITE setError DESIGNABLE true )
    Q_ENUMS( ScriptInput )
    Q_ENUMS( ScriptOutput )

public:
    /**
     * What we send to the script.
     */
    enum ScriptInput {
        NoInput,
        Selection,
        Document
    };

    /**
     * What the script sends us.
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
                        QObject* parent, const char* name,
                        QuantaView *view );

    /**
     * Create an insert with an icon.
     */
    QuantaScriptAction( const QString& text, const QString& pix, int accel,
                        QObject* parent, const char* name,
                        QuantaView *view );

    virtual ~QuantaScriptAction();

    /**
     * Set the command to be executed. If the string <i>%f</i> is in the
     * command, it will be expanded to the filename to be processed.
     */
    void setScript( const QString &file ) { filename_ = file; }
    QString script() const { return filename_; }

    /**
     * Set the data that should be sent to the script.
     */
    void setInput( const ScriptInput in ) { this->input_ = in; }
    ScriptInput input() const { return input_; }

    /**
     * Specify what should be done with the output of the script.
     */
    void setOutput( const ScriptOutput out ) { this->output_ = out; }
    ScriptOutput output() const { return output_; }

    /**
     * Specify what should be done with error messages from the script.
     */
    void setError( const ScriptOutput err ) { this->error_ = err; }
    ScriptOutput error() const { return error_; }

protected slots:
    virtual void runScript();
    virtual void slotGetScriptOutput( KProcess *, char *buffer, int buflen );
    virtual void slotGetScriptError( KProcess *, char *buffer, int buflen );
    virtual void scriptDone();

private:
    QString filename_;
    ScriptInput input_;
    ScriptOutput output_;
    ScriptOutput error_;
    KProcess *proc;
    bool firstError;
    bool firstOutput;
    QuantaView *view_;
};

#endif // QUANTA_ACTIONS_H
