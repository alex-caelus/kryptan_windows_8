#pragma once

#include "BaseModel.h"
#include "CustomViews\SecureTextEdit.xaml.h"

class DecryptModel : public BaseModel
{
public:

    struct DecryptResult
    {
        bool success;
        Platform::String^ statusString;
    };

    /// <summary>
    /// Initializes a new instance of the <see cref="DecryptModel" /> class.
    /// </summary>
    DecryptModel();

    /// <summary>
    /// Finalizes an instance of the <see cref="DecryptModel" /> class.
    /// </summary>
    ~DecryptModel();

    /// <summary>
    /// Decrypt button clicked.
    /// </summary>
    /// <param name="masterkeyRaw">The raw masterkey.</param>
    /// <returns>an async task</returns>
    concurrency::task<DecryptModel::DecryptResult> decryptButtonClicked(kryptan_windows::SecureStringContainer^ masterkey);

    /// <summary>
    /// Doeses the password file exist.
    /// </summary>
    /// <returns></returns>
    bool doesPwdFileExist();

private:
    Caelus::Utilities::SecureString* newMasterkey;

};

