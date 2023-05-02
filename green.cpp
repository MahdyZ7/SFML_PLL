#include <SFML/Graphics.hpp>
#include <math.h>
#include <unistd.h>

#define screen_width 1000
#define screen_height 500
#define screen_center_x screen_width / 2
#define screen_center_y screen_height / 2
#define screen_third_x screen_width / 3
#define screen_third_y screen_height / 3

// Set global parameters for the sine wave
float	time1 = 0.0;           
float	amplitude = 150.0;        
float	frequency = 1.0;

void DrawSineWave(sf::RenderWindow& window) {
    sf::VertexArray line(sf::LineStrip, 1000);
	sf::VertexArray line1(sf::LineStrip, 500);
    for (int x = 0; x < 1000; x++) {
        float y = sin(frequency * (x - time1) / 100.0) * amplitude;
        line[x] = sf::Vertex(sf::Vector2f(x, y + screen_center_y));
        line[x].color = sf::Color::Blue;
	}
	for (int x = 0; x < 500; x++) {
        float y = sin(frequency * (x - time1) / 100.0) * amplitude;
		line1[x] = sf::Vertex(sf::Vector2f(x, y + screen_center_y));
        line1[x].color = sf::Color::White;
    }
    window.draw(line);
    window.draw(line1);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Moving Sine Wave");


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);

        DrawSineWave(window);
		usleep(1000);
        window.display();
        time1 += 1.0;
    }

    return 0;
}

/*
// Define the state vector
Eigen::VectorXd x(2);
x(0) = initial_phase;       // Initial phase
x(1) = initial_frequency;   // Initial frequency deviation

// Define the state transition matrix
Eigen::MatrixXd F(2, 2);
F << 1, Ts, 0, 1;     // Assumes constant frequency

// Define the measurement matrix
Eigen::MatrixXd H(1, 2);
H << 1, 0;             // Measure phase only

// Define the process noise covariance matrix
Eigen::MatrixXd Q(2, 2);
Q << sigma_phase, 0, 0, sigma_freq;

// Define the measurement noise covariance
Eigen::MatrixXd R(1, 1);
R << sigma_phase;

// Initialize the Kalman filter
Eigen::MatrixXd P(2, 2);
P.setZero();
P.diagonal() << initial_phase_var, initial_freq_var;   // Initial error covariance

// Execute the loop
for (int i = 0; i < num_iterations; ++i) {
    // Predict the state estimate
    x = F * x;
    P = F * P * F.transpose() + Q;

    // Update based on new measurement
    Eigen::VectorXd y(1);
    y << phase_error_measurement;
    Eigen::MatrixXd K = P * H.transpose() * (H * P * H.transpose() + R).inverse();
    x = x + K * (y - H * x);
    P = (Eigen::MatrixXd::Identity(2, 2) - K * H) * P;
    
    // Compute PLL output
    double pll_output = cos(2 * pi * (fc + x(1)) * t + x(0));
}
*/