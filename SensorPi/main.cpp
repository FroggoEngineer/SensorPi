#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <string>



int main()
{

	sf::RenderWindow window(sf::VideoMode(600, 600), "Sensor Project");

	sf::Texture diag1;
	sf::Texture diag2;
	sf::Texture diag3;

	if (!diag1.loadFromFile("diagram1.png")) {
		//error
	}
	if (!diag2.loadFromFile("diagram2.png")) {
		//error
	}
	if (!diag3.loadFromFile("diagram3.png")) {
		//error
	}

	sf::Sprite sprite;

	std::vector<float> tempPoints;
	std::vector<float> humPoints;
	std::vector<float> presPoints;

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		sprite.setTexture(diag1);
		sprite.setPosition(0, 0);
		window.draw(sprite);

		sprite.setTexture(diag2);
		sprite.setPosition(0, 200);
		window.draw(sprite);

		sprite.setTexture(diag3);
		sprite.setPosition(0, 400);
		window.draw(sprite);
		/*
			Origo of diagram: 31, 167
			top of diagram: 31, 3

			max temp: 40
			min temp: 0

			height: 164pix
			scale: 164/40 = 4.1

			temp height: 164 - temp*4.1


			hum scale: 164/100 = 1.64
			hum height: 364 - hum*1.64

			pressure scale: 164/20 = 8.2
			pressure height: 564 - (pres-990)*8.2

			111 data points can be shown at this time

		*/

		if(tempPoints.size() >= 2)
			for (int i{ 0 }; i < tempPoints.size()-1; ++i) {
				sf::VertexArray line(sf::Lines, 2);
				line[0].position = sf::Vector2f(i * 5 + 32, 164 - ((int)(tempPoints[i] * 4.1)));
				line[1].position = sf::Vector2f((i+1) * 5 + 32, 164 - ((int)(tempPoints[i+1] * 4.1)));

				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;

				window.draw(line);
			}

		if (humPoints.size() >= 2)
			for (int i{ 0 }; i < presPoints.size() - 1; ++i) {
				sf::VertexArray line(sf::Lines, 2);
				line[0].position = sf::Vector2f(i * 5 + 32, 364 - ((int)(humPoints[i] * 1.64)));
				line[1].position = sf::Vector2f((i + 1) * 5 + 32, 364 - ((int)(humPoints[i + 1] * 1.64)));

				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;

				window.draw(line);
			}

		if (presPoints.size() >= 2)
			for (int i{ 0 }; i < presPoints.size() - 1; ++i) {
				sf::VertexArray line(sf::Lines, 2);
				line[0].position = sf::Vector2f(i * 5 + 32, 564 - ((int)((presPoints[i]-990) * 8.2)));
				line[1].position = sf::Vector2f((i + 1) * 5 + 32, 564 - ((int)((presPoints[i + 1]-990) * 8.2)));

				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;

				window.draw(line);
			}

		window.display();


		sf::TcpListener listener;


		if (listener.listen(1001) != sf::Socket::Done) {

		}

		sf::TcpSocket client;


		if (listener.accept(client) != sf::Socket::Done) {

		}

		int dataSize{ 8 };
		std::size_t receivedSize;
		
		
		


		//Temperature
		//----------------------------------------------
		char data[100];
		if (client.receive(data, dataSize, receivedSize) != sf::Socket::Done) {

		}

		//std::cout << "Received " << receivedSize << " bytes" << std::endl;

		std::string num;

		for (int i{ 0 }; i < dataSize; ++i) {
			num += data[i];
		}

		float temperature = std::stof(num);

		std::cout << "Temperature: " << temperature << std::endl;

		tempPoints.push_back(temperature);

		
		//----------------------------------------------


		//Humidity
		//----------------------------------------------
		char data1[100];

		if (client.receive(data1, dataSize, receivedSize) != sf::Socket::Done) {

		}

		//std::cout << "Received " << receivedSize << " bytes" << std::endl;

		//std::cout << data1 << std::endl;

		num = "";

		for (int i{ 0 }; i < dataSize; ++i) {
			num += data1[i];
		}


		float humidity = std::stof(num);

		std::cout << "Humidity: " << humidity << std::endl;

		humPoints.push_back(humidity);

		//----------------------------------------------


		//Air pressure
		//----------------------------------------------
		char data2[100];

		if (client.receive(data2, dataSize, receivedSize) != sf::Socket::Done) {

		}

		//std::cout << "Received " << receivedSize << " bytes" << std::endl;

		//std::cout << data2 << std::endl;

		num = "";

		for (int i{ 0 }; i < dataSize; ++i) {
			num += data2[i];
		}


		float pressure = std::stof(num);

		std::cout << "Pressure: " << pressure << std::endl;

		presPoints.push_back(pressure);
		client.disconnect();
		//----------------------------------------------
		
		



	}


	return 0;
}