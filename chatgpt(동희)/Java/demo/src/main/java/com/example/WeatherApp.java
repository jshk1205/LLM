package com.example;

import org.json.JSONArray;
import org.json.JSONObject;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

public class WeatherApp extends JFrame {
    private static final String API_KEY = "0f6019d136f47fffc0181d5e61728bb5";
    private static final String API_URL = "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric";

    private JTextField cityTextField;
    private JTextArea weatherTextArea;

    public WeatherApp() {
        setTitle("날씨 애플리케이션");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());

        cityTextField = new JTextField();
        panel.add(cityTextField, BorderLayout.NORTH);

        JButton searchButton = new JButton("검색");
        searchButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String cityName = cityTextField.getText().trim();
                if (!cityName.isEmpty()) {
                    fetchWeatherData(cityName);
                }
            }
        });
        panel.add(searchButton, BorderLayout.EAST);

        weatherTextArea = new JTextArea();
        panel.add(new JScrollPane(weatherTextArea), BorderLayout.CENTER);

        add(panel);
        setVisible(true);
    }

    private void fetchWeatherData(String cityName) {
        try {
            String encodedCityName = URLEncoder.encode(cityName, "UTF-8");
            String apiUrl = String.format(API_URL, encodedCityName, API_KEY);
            URL url = new URL(apiUrl);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            StringBuilder response = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                response.append(line);
            }
            reader.close();
            connection.disconnect();

            JSONObject jsonResponse = new JSONObject(response.toString());
            String weather = jsonResponse.getJSONArray("weather").getJSONObject(0).getString("description");
            JSONObject main = jsonResponse.getJSONObject("main");
            double temperature = main.getDouble("temp");

            String weatherInfo = String.format("도시: %s\n날씨: %s\n온도: %.1f°C", cityName, weather, temperature);
            weatherTextArea.setText(weatherInfo);
        } catch (IOException ex) {
            ex.printStackTrace();
            weatherTextArea.setText("날씨 정보를 가져오는 중 오류가 발생했습니다.");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new WeatherApp();
            }
        });
    }
}
