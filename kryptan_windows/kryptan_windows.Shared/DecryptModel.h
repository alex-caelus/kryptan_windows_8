#pragma once

#include "BaseModel.h"
#include "SecureStringHandler.h"

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
    concurrency::task<DecryptModel::DecryptResult> decryptButtonClicked(Platform::String^ masterkeyRaw);

    /// <summary>
    /// Doeses the password file exist.
    /// </summary>
    /// <returns></returns>
    bool doesPwdFileExist();

private:
    SecureStringHandler* newMasterkey;

};

