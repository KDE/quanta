/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XMLVALIDATORJOB_H
#define XMLVALIDATORJOB_H

#include "xmlvalidator.h"

#include <KDE/KJob>

/** Validates a document against a schema or dtd document.
 *  If the dtd or schema is not a local file it will make a local copy.
 *  <pre>
 *  XmlValidatorJob * job = XmlValidatorJob::dtdValidationJob("test.xml", "test.dtd");
 *  connect(job, SIGNAL(result(KJob *)), this, slot(slotValidated(KJob *)));
 *  Kdevelop::ICore::self()->runController()->registerJob(job);
 *  </pre>
 */
class XmlValidatorJob : public KJob, protected XmlValidator {
    Q_OBJECT
public:
    void start();

    /** The result of the validation. */
    XmlValidator::ValidationResult getResult() const {
        return m_result;
    }

    /** The list of errors that occurred. */
    const QStringList & errors() const {
        return XmlValidator::errors();
    }

    /** The list of warnings that occurred. */
    const QStringList & warnings() const {
        return XmlValidator::warnings();
    }

    /** The URL of the document being validated. */
    const QString& documentUrl() const {
        return m_documentUrl;
    }

    /** The DTD validated against or QString::null. */
    const QString& dtdUrl() const {
        return m_dtdUrl;
    }

    /** The Schema validated against or QString::null. */
    const QString& schemaUrl() const {
        return m_schemaUrl;
    }

    /** A concatenation of errors and warnings. */
    virtual QString errorString() const;

    /** Create a new job to validate against a DTD. */
    static XmlValidatorJob* dtdValidationJob(const QString &documentUrl, const QString &dtd = QString());

    /** Create a new job to validate against a schema. */
    static XmlValidatorJob* schemaValidationJob(const QString &documentUrl, const QString &schema);
    
protected:
    XmlValidatorJob(QObject* parent = 0);
    virtual ~XmlValidatorJob();
    QString getLocalURLForSchema ( const QString& file, const QString& schema );
signals:
    //Emitted when the files are read for validation.
    void signalReady(KJob *);
private slots:
    void ready(KJob *);
private:
    void validate();

    ValidationResult m_result;
    QString m_documentUrl;
    QString m_dtdUrl;
    QString m_schemaUrl;
};

#endif //XMLVALIDATORJOB_H


