#include "pch.h"
#include "BaseModel.h"

using namespace kryptan_windows;

/// <summary>
/// The password file
/// </summary>
Kryptan::Core::PwdFile* BaseModel::pwdFile;

/// <summary>
/// Initializes a new instance of the <see cref="BaseModel"/> class.
/// </summary>
BaseModel::BaseModel()
{
}


/// <summary>
/// Finalizes an instance of the <see cref="BaseModel"/> class.
/// </summary>
BaseModel::~BaseModel()
{
}

/// <summary>
/// Determines whether password is decrypted.
/// </summary>
/// <returns></returns>
bool BaseModel::isPwdDecrypted()
{
    return pwdFile != NULL && pwdFile->IsOpen();
}