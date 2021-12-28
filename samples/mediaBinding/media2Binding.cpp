//
// Created by li.tsunglin on 2021/12/28.
//

#include <iostream>
#include <string>
#include <stdsoap2.h>
#include "wsseapi.h"
#include "soapMedia2BindingProxy.h"
#include "common.h"

using namespace std;

const string username = USERNAME;
const string password =  PASSWORD;
const string hostname =  HOSTNAME;

vector<pair<string, string>> getProfiles() {

//    std::cout << "====================== MediaBinding Profiles ======================" << std::endl;

    Media2BindingProxy Media2BindingProxy;
    Media2BindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(Media2BindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(Media2BindingProxy.soap);
        return vector<pair<string, string>>();
    }

    struct soap* soap = soap_new();
    _ns3__GetProfiles *GetProfiles = soap_new__ns3__GetProfiles(soap);
    _ns3__GetProfilesResponse *GetProfilesResponse = soap_new__ns3__GetProfilesResponse(soap);

    vector<pair<string, string>> profiles;
    cout << "Profile name" << "         " << "Profile token" << endl;
    if (SOAP_OK == Media2BindingProxy.GetProfiles(GetProfiles, *GetProfilesResponse)) {
        for (auto & Profile : GetProfilesResponse->Profiles) {
            cout << Profile->Name << "         " << Profile->token << endl;
            profiles.emplace_back(pair<string, string>(Profile->Name, Profile->token));
        }
    } else {
        std::cerr <<"Error: getProfiles" << endl;
        report_error(Media2BindingProxy.soap);
    }

    CLEANUP_SOAP(soap);

    return profiles;
}

void getSnapshotUri(string &profileToken) {

//    std::cout << "====================== MediaBinding GetSnapshotUri ======================" << std::endl;

    Media2BindingProxy Media2BindingProxy;
    Media2BindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(Media2BindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(Media2BindingProxy.soap);
        return;
    }

    struct soap* soap = soap_new();
    _ns3__GetSnapshotUri *GetSnapshotUri = soap_new__ns3__GetSnapshotUri(soap);
    GetSnapshotUri->ProfileToken = profileToken;
    _ns3__GetSnapshotUriResponse *GetSnapshotUriResponse = soap_new__ns3__GetSnapshotUriResponse(soap);

    if (SOAP_OK == Media2BindingProxy.GetSnapshotUri(GetSnapshotUri, *GetSnapshotUriResponse)) {
        cout << "SnapshotUri: " << GetSnapshotUriResponse->Uri << endl;
    } else {
        std::cerr <<"Error: getSnapshotUri" << endl;
        report_error(Media2BindingProxy.soap);
    }

    CLEANUP_SOAP(soap);
}

void getStreamUri(string &profileToken) {

//    std::cout << "====================== MediaBinding GetStreamUri ======================" << std::endl;

    Media2BindingProxy Media2BindingProxy;
    Media2BindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(Media2BindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(Media2BindingProxy.soap);
        return;
    }

    struct soap* soap = soap_new();
    _ns3__GetStreamUri *GetStreamUri = soap_new__ns3__GetStreamUri(soap);
    GetStreamUri->ProfileToken = profileToken;
    _ns3__GetStreamUriResponse *GetStreamUriResponse = soap_new__ns3__GetStreamUriResponse(soap);

    if (SOAP_OK == Media2BindingProxy.GetStreamUri(GetStreamUri, *GetStreamUriResponse)) {
        cout << "StreamUri: " << GetStreamUriResponse->Uri << endl;
    } else {
        std::cerr <<"Error: getStreamUri" << endl;
        report_error(Media2BindingProxy.soap);
    }

    CLEANUP_SOAP(soap);
}

int main() {
    std::cout << "====================== Media2Binding Profiles ======================" << std::endl;
    vector<pair<string, string>> profiles = getProfiles();

    if (!profiles.empty()) {
        std::cout << "====================== Media2Binding GetSnapshotUri ======================" << std::endl;
        getSnapshotUri(profiles[0].second);

        std::cout << "====================== Media2Binding GetStreamUri ======================" << std::endl;
        getStreamUri(profiles[0].second);
    }
    
    

    return 0;
}