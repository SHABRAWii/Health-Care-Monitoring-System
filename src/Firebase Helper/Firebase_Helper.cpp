#include "Firebase_Helper.h"

FirebaseHelper::FirebaseHelper(const char* apiKey, const char* email, 
const char* emailPassword, const char* databaseURL){
    taskComplete = false; 
    aClient = new AsyncClient(ssl_client, getNetwork(network));
    
    user_auth = new UserAuth(apiKey, email, emailPassword);
    ssl_client.setInsecure();
    initializeApp(*aClient, app, getAuth(*user_auth), aResult_no_callback);
    app.getApp<RealtimeDatabase>(database);
    database.url(databaseURL);
}

void FirebaseHelper::Init() {
    
    
}
void FirebaseHelper::loop() {
    app.loop();

    database.loop();

    if (app.ready() && !taskComplete)
    {
    //     taskComplete = true;

    //     Serial.println("Asynchronous Push... ");

    //     // Push int
    //     database.push<int>(*aClient, "/test/int", 12345, aResult_no_callback);

    //     // Push bool
    //     database.push<bool>(*aClient, "/test/bool", true, aResult_no_callback);

    //     // Push string
    //     database.push<String>(*aClient, "/test/string", "hello", aResult_no_callback);
    }

    printResult(aResult_no_callback);
}
void FirebaseHelper::printResult(AsyncResult &aResult)
{
    if (aResult.isEvent())
    {
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isDebug())
    {
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }

    if (aResult.available())
    {
        if (aResult.to<RealtimeDatabaseResult>().name().length())
            Firebase.printf("task: %s, name: %s\n", aResult.uid().c_str(), aResult.to<RealtimeDatabaseResult>().name().c_str());
        Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    }
}

void FirebaseHelper::pushFloat(String path, float value) {
    
    
    // Push the value to the new path
    database.set<float>(*aClient, path.c_str(), value, aResult_no_callback);
    // taskComplete = true;
}

void FirebaseHelper::pushString(String path, String value) {
    database.set<String>(*aClient, path.c_str(), value, aResult_no_callback);
    // taskComplete = true;
}

// void FirebaseHelper::pushString(const char* path, const char* value) {
//     database.push<String>(aClient, path, value, aResult_no_callback);
//     taskComplete = true;
// }

// FirebaseHelper::FirebaseHelper(const char* firebaseId, bool _DEBUG) :  DEBUG(_DEBUG) {
//     for(int i = 0 ; i < 20 ; ++i){
//         tmpData[i] = -1;
//     }
// }


// void FirebaseHelper::sendToFirebase(String Header, String message) {
//     String key = "", tmp = "";
//     for(int i = 0 ; i < message.length() ; ++i){
//         if(message[i] == ':'){  
//             key = tmp;
//             tmp = "";
//         } else if(message[i] == ';'){
//             if(DEBUG){
//                 Serial.println("[Sending]" + Header + "[" + key + "] = " + tmp + " ... ");
//             }
//             if(Header == "."){
//                 // firebase.setFloat(key, atof(tmp.c_str()));
//             }else{
//                 // firebase.setFloat(Header + "/" + key, atof(tmp.c_str()));

//             }
//             if(DEBUG){
//               Serial.println("[Success]" + Header + "[" + key + "] = " + tmp);
//             }
//             tmp = "";
//         } else {
//             tmp += message[i];
//         }
//     }
// }
