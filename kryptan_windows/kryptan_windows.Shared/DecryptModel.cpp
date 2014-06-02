#include "pch.h"
#include "DecryptModel.h"
#include "SecureStringHandler.h"

using namespace concurrency;
using namespace Platform;

/// <summary>
/// Initializes a new instance of the <see cref="DecryptModel" /> class.
/// </summary>
DecryptModel::DecryptModel()
{
}


/// <summary>
/// Finalizes an instance of the <see cref="DecryptModel" /> class.
/// </summary>
DecryptModel::~DecryptModel()
{
}


/// <summary>
/// Decrypts the button clicked.
/// </summary>
/// <param name="masterkeyRaw">The masterkey raw.</param>
task<DecryptModel::DecryptResult> DecryptModel::decryptButtonClicked(Platform::String^ masterkeyRaw)
{
    return create_task([this, masterkeyRaw]{
        try{
            auto masterkeyHandler = new SecureStringHandler(masterkeyRaw);

            if (pwdFile == NULL)
            {
                Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
                Platform::String^ filename = ref new Platform::String(L"/secret.pwd");
                std::wstring fullPath = std::wstring((localFolder->Path + filename)->Data());

                Kryptan::Core::PwdFile* file = new Kryptan::Core::PwdFile(std::string(fullPath.begin(), fullPath.end()));
            }
            if (pwdFile->Exists())
            {
                pwdFile->OpenAndParse(masterkeyHandler->getCorrespondingSecureString());
            }
            else
            {
                if (newMasterkey == NULL)
                {
                    //we want the user to confirm it before applying it
                    DecryptResult res;
                    res.success = true;
                    res.statusString = "Input Masterkey again to confirm!";
                    return res;
                }
                else if (newMasterkey->getCorrespondingSecureString().equals(masterkeyHandler->getCorrespondingSecureString()))
                {
                    pwdFile->CreateNew();
                    if (pwdFile->IsOpen())
                        pwdFile->Save(masterkeyHandler->getCorrespondingSecureString());
                }
                else
                {
                    //Confirmation failed
                    newMasterkey = NULL;
                    DecryptResult res;
                    res.success = false;
                    res.statusString = "The keys did not match, please try again!";
                    return res;
                }
            }

            DecryptResult res;
            res.success = pwdFile->IsOpen();
            res.statusString = ref new String(res.success ? L"Success!" : L"Unknown failure!");

            return res;
        }
        catch (std::exception& e)
        {
            DecryptResult res;
            res.success = false;
            res.statusString = ref new String(L"Failure: ") + SecureStringHandler::makePlatformString(e.what());
            return res;
        }
    });
}


/// <summary>
/// Doeses the password file exist.
/// </summary>
/// <returns></returns>
bool DecryptModel::doesPwdFileExist()
{
    return pwdFile != NULL && pwdFile->Exists();
}