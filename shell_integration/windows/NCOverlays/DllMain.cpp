/**
 * Copyright (c) 2000-2013 Liferay, Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */

#include "NCOverlayRegistrationHandler.h"
#include "NCOverlayFactory.h"
#include "WinShellExtConstants.h"
#include <fstream>
#include "utils.h"

HINSTANCE instanceHandle = nullptr;

long dllReferenceCount = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    std::ofstream outfile;
    outfile.open(logsFileName().c_str(), std::ios_base::app);
    outfile << "DllMain..." << "\r\n";
    outfile.close();
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            instanceHandle = hModule;
            DisableThreadLibraryCalls(hModule);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

HRESULT CreateFactory(REFIID riid, void **ppv, int state)
{
    HRESULT hResult = E_OUTOFMEMORY;
    std::ofstream outfile;
    outfile.open(logsFileName().c_str(), std::ios_base::app);
    outfile << "CreateFactory..." << "\r\n";
    outfile.close();

    NCOverlayFactory* ncOverlayFactory = new NCOverlayFactory(state);

    if (ncOverlayFactory) {
        hResult = ncOverlayFactory->QueryInterface(riid, ppv);
        ncOverlayFactory->Release();
    }
    return hResult;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    HRESULT hResult = CLASS_E_CLASSNOTAVAILABLE;
    GUID guid;

    std::ofstream outfile;
    outfile.open(logsFileName().c_str(), std::ios_base::app);
    outfile << "DllGetClassObject..." << "\r\n";
 
    hResult = CLSIDFromString(OVERLAY_GUID_ERROR, (LPCLSID)&guid);
    if (!SUCCEEDED(hResult)) {
        OLECHAR *guidString;
        StringFromCLSID(guid, &guidString);

        // use guidString...

        // ensure memory is freed
        outfile << "State_Error !SUCCEEDED(hResult) hResult: " << hResult << " guid : " << guidString << "\r\n ";
        outfile.close();

        ::CoTaskMemFree(guidString);
        return hResult;
    }
    if (IsEqualCLSID(guid, rclsid)) {
        OLECHAR *guidString;
        StringFromIID(rclsid, &guidString);
        outfile << "State_Error IsEqualCLSID guid: " << (LPCLSID)&guid << " rclsid: " << guidString << "\r\n";
        outfile.close();
        ::CoTaskMemFree(guidString);
        return CreateFactory(riid, ppv, State_Error);
    }

    hResult = CLSIDFromString(OVERLAY_GUID_OK, (LPCLSID)&guid);
    if (!SUCCEEDED(hResult)) {
        OLECHAR *guidString;
        StringFromCLSID(guid, &guidString);
        outfile << "State_OK !SUCCEEDED(hResult) hResult: " << hResult << " guid : " << guidString << "\r\n";
        outfile.close();
        ::CoTaskMemFree(guidString);
        return hResult;
    }
    if (IsEqualCLSID(guid, rclsid)) {
        OLECHAR *guidString;
        StringFromIID(rclsid, &guidString);
        outfile << "State_OK IsEqualCLSID guid: " << (LPCLSID)&guid << " rclsid: " << guidString << "\r\n";
        outfile.close();
        ::CoTaskMemFree(guidString);
        return CreateFactory(riid, ppv, State_OK);
    }

    hResult = CLSIDFromString(OVERLAY_GUID_OK_SHARED, (LPCLSID)&guid);
    if (!SUCCEEDED(hResult)) {
        OLECHAR *guidString;
        StringFromCLSID(guid, &guidString);
        outfile << "State_OKShared !SUCCEEDED(hResult) hResult: " << hResult << " guid : " << guidString << "\r\n";
        outfile.close();
        ::CoTaskMemFree(guidString);
        return hResult;
    }
    if (IsEqualCLSID(guid, rclsid)) {
        OLECHAR *guidString;
        StringFromIID(rclsid, &guidString);
        outfile << "State_OKShared IsEqualCLSID guid: " << (LPCLSID)&guid << " rclsid: " << guidString << "\r\n";
        outfile.close();
        ::CoTaskMemFree(guidString);
        return CreateFactory(riid, ppv, State_OKShared);
    }

    hResult = CLSIDFromString(OVERLAY_GUID_SYNC, (LPCLSID)&guid);
    if (!SUCCEEDED(hResult)) {
        outfile << "State_Sync !SUCCEEDED(hResult) hResult: " << hResult << " guid : " << (LPCLSID)&guid << "\r\n";
        outfile.close();
        return hResult;
    }
    if (IsEqualCLSID(guid, rclsid)) {
        outfile << "State_Sync IsEqualCLSID guid: " << (LPCLSID)&guid << " rclsid: " << (LPCLSID)&rclsid << "\r\n";
        outfile.close();
        return CreateFactory(riid, ppv, State_Sync);
    }

    hResult = CLSIDFromString(OVERLAY_GUID_WARNING, (LPCLSID)&guid);
    if (!SUCCEEDED(hResult)) {
        outfile << "State_Warning !SUCCEEDED(hResult) hResult: " << hResult << " guid : " << (LPCLSID)&guid << "\r\n";
        outfile.close();
        return hResult;
    }
    if (IsEqualCLSID(guid, rclsid)) {
        outfile << "State_Warning IsEqualCLSID guid: " << (LPCLSID)&guid << " rclsid: " << (LPCLSID)&rclsid << "\r\n";
        outfile.close();
        return CreateFactory(riid, ppv, State_Warning);
    }

    return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI DllCanUnloadNow(void)
{
    return dllReferenceCount > 0 ? S_FALSE : S_OK;
}

HRESULT RegisterCLSID(LPCOLESTR guidStr, PCWSTR overlayStr, PCWSTR szModule)
{
    std::ofstream outfile;
    outfile.open(logsFileName().c_str(), std::ios_base::app);
    outfile << "RegisterCLSID..." << "\r\n";
    outfile.close();

    HRESULT hResult = S_OK;

    GUID guid;
    hResult = CLSIDFromString(guidStr, (LPCLSID)&guid);

    if (hResult != S_OK) {
        return hResult;
    }

    hResult = NCOverlayRegistrationHandler::RegisterCOMObject(szModule, OVERLAY_DESCRIPTION, guid);

    if (!SUCCEEDED(hResult)) {
        return hResult;
    }

    hResult = NCOverlayRegistrationHandler::MakeRegistryEntries(guid, overlayStr);

    return hResult;
}

HRESULT UnregisterCLSID(LPCOLESTR guidStr, PCWSTR overlayStr)
{
    HRESULT hResult = S_OK;
    GUID guid;

    hResult = CLSIDFromString(guidStr, (LPCLSID)&guid);

    if (hResult != S_OK) {
        return hResult;
    }

    hResult = NCOverlayRegistrationHandler::UnregisterCOMObject(guid);

    if (!SUCCEEDED(hResult)) {
        return hResult;
    }

    hResult = NCOverlayRegistrationHandler::RemoveRegistryEntries(overlayStr);

    return hResult;
}

HRESULT _stdcall DllRegisterServer(void)
{
    HRESULT hResult = S_OK;

    wchar_t szModule[MAX_PATH];

    if (GetModuleFileName(instanceHandle, szModule, ARRAYSIZE(szModule)) == 0) {
        hResult = HRESULT_FROM_WIN32(GetLastError());
        return hResult;
    }

    hResult = RegisterCLSID(OVERLAY_GUID_ERROR, OVERLAY_NAME_ERROR, szModule);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = RegisterCLSID(OVERLAY_GUID_OK, OVERLAY_NAME_OK, szModule);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = RegisterCLSID(OVERLAY_GUID_OK_SHARED, OVERLAY_NAME_OK_SHARED, szModule);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = RegisterCLSID(OVERLAY_GUID_SYNC, OVERLAY_NAME_SYNC, szModule);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = RegisterCLSID(OVERLAY_GUID_WARNING, OVERLAY_NAME_WARNING, szModule);

    return hResult;
}

STDAPI DllUnregisterServer(void)
{
    HRESULT hResult = S_OK;

    wchar_t szModule[MAX_PATH];
    
    if (GetModuleFileNameW(instanceHandle, szModule, ARRAYSIZE(szModule)) == 0)
    {
        hResult = HRESULT_FROM_WIN32(GetLastError());
        return hResult;
    }

    hResult = UnregisterCLSID(OVERLAY_GUID_ERROR, OVERLAY_NAME_ERROR);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = UnregisterCLSID(OVERLAY_GUID_OK, OVERLAY_NAME_OK);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = UnregisterCLSID(OVERLAY_GUID_OK_SHARED, OVERLAY_NAME_OK_SHARED);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = UnregisterCLSID(OVERLAY_GUID_SYNC, OVERLAY_NAME_SYNC);
    if (!SUCCEEDED(hResult)) { return hResult; }
    hResult = UnregisterCLSID(OVERLAY_GUID_WARNING, OVERLAY_NAME_WARNING);

    return hResult;
}
