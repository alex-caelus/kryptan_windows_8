#pragma once

#include "Libs\kryptan_core\core.h"

class BaseModel
{
public:

    /// <summary>
    /// The password file
    /// </summary>
    BaseModel();

    /// <summary>
    /// Finalizes an instance of the <see cref="BaseModel"/> class.
    /// </summary>
    ~BaseModel();

    /// <summary>
    /// Determines whether password is decrypted.
    /// </summary>
    /// <returns></returns>
    bool isPwdDecrypted();

protected:

    static Kryptan::Core::PwdFile* pwdFile;
};

