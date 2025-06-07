# Weather Forecast in C++
 
This is a simple console-based weather forecast application built in **C++** using:

- **cURL** for making HTTP requests
- **OpenWeatherMap API** for weather data
- **nlohmann/json** for JSON parsing
- **File Handling** to save forecasts in a `data.json` file

##  Features

- User inputs the city name and number of forecast days (1 to 5).
- Fetches weather forecast every 3 hours from OpenWeatherMap.
- Displays and saves:
  - Date & time
  - Temperature
  - Humidity
  - Weather description with emoji 
- Forecast is saved to a structured `data.json` file.

## 🖥️ Sample Output

Enter city name: London

Enter number of forecast days (1-5): 3

Weather forecast saved to data.json successfully

🔧 Requirements
C++ compiler (e.g. g++)

cURL library (with libcurl.dll)

nlohmann/json header file


