#ifndef __ACTIONCONFIGDIALOG_INTERFACE_H
#define __ACTIONCONFIGDIALOG_INTERFACE_H

class ActionConfigDialogInterface
{
public:
    ActionConfigDialogInterface() {}
    virtual ~ActionConfigDialogInterface() {}

    virtual void accept() = 0;
    virtual void reject() = 0;

private:
    ActionConfigDialogInterface( const ActionConfigDialogInterface& source );
    void operator = ( const ActionConfigDialogInterface& source );
};


#endif // __ACTIONCONFIGDIALOG_INTERFACE_H
