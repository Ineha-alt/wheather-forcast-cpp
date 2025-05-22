#include <iostream>
#include <fstream>  
#include <curl/curl.h>
#include "json.hpp"  
using json = nlohmann::json;
using namespace std;

// Function to handle HTTP response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Function to map weather descriptions to emojis
string getWeatherEmoji(string weatherDesc) {
    if (weatherDesc.find("clear") != string::npos) return "☀️";
    if (weatherDesc.find("cloud") != string::npos) return "☁️";
    if (weatherDesc.find("rain") != string::npos) return "🌧️";
    if (weatherDesc.find("thunderstorm") != string::npos) return "⛈️";
    if (weatherDesc.find("snow") != string::npos) return "❄️";
    if (weatherDesc.find("fog") != string::npos || weatherDesc.find("mist") != string::npos) return "🌫️";
    return "🌡️";  
}

int main() {
    string city;
    int days;
    cout << "Enter city name: ";
    getline(cin, city);

    cout << "Enter number of forecast days (1-5): ";
    cin >> days;

    if (days < 1 || days > 5) {
        cout << "Invalid input! Showing 5-day forecast.\n";
        days = 5;
    }

    string apiKey = "5cb90eb1ec43dfc679332cab6617f990";  
    string url = "http://api.openweathermap.org/data/2.5/forecast?q=" + city + "&appid=" + apiKey + "&units=metric";

    CURL* curl;
    CURLcode res;
    string response;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            try {
                json weatherData = json::parse(response);
                json outputJson;

                outputJson["city"] = city;
                outputJson["forecast"] = json::array();

                
                int dataPointsPerDay = 8; 
                int totalDataPoints = days * dataPointsPerDay;

                for (size_t i = 0; i < totalDataPoints && i < weatherData["list"].size(); i += 8) {
                    string weatherDesc = weatherData["list"][i]["weather"][0]["description"];
                    
                    json dailyForecast;
                    dailyForecast["date"] = weatherData["list"][i]["dt_txt"];
                    dailyForecast["temperature"] = weatherData["list"][i]["main"]["temp"];
                    dailyForecast["humidity"] = weatherData["list"][i]["main"]["humidity"];
                    dailyForecast["weather"] = weatherDesc + " " + getWeatherEmoji(weatherDesc); 

                    outputJson["forecast"].push_back(dailyForecast);
                }

                
                ofstream jsonFile("data.json");
                jsonFile << outputJson.dump(4);  
                jsonFile.close();

                cout << "Weather forecast saved to data.json successfully!" << endl;

            } catch (json::parse_error& e) {
                cout << "Error parsing JSON data." << endl;
            }
        } else {
            cout << "Error fetching weather data!" << endl;
        }
    } else {
        cout << "Failed to initialize cURL." << endl;
    }

    curl_global_cleanup();
    return 0;
}
