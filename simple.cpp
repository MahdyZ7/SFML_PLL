#include <SFML/Graphics.hpp>
#include <complex.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define screen_width 1000
#define screen_height 500
#define screen_center_x screen_width / 2
#define screen_center_y screen_height / 2
#define screen_third_x screen_width / 3
#define screen_third_y screen_height / 3

// parameters and simulation options
float	phase_in =		3.0f;    // carrier phase offset
float	frequency_in =	-0.02;    // carrier frequency offset
float	alpha =			0.05f;   // phase adjustment factor

// initialize states
float beta = 			0.5*alpha*alpha; // frequency adjustment factor
float phase_out =		0.0f;            // output signal phase
float frequency_out =	0.0f;            // output signal frequency


// Set global parameters for the sine wave
float	time1 = 0.0;           
float	amplitude = 150.0;        
float	frequency = 1.0;

float complex signal_in[1000], signal_out[1000];

void DrawSineWave(sf::RenderWindow& window) {
    sf::VertexArray line(sf::LineStrip, 1000);
	sf::VertexArray line1(sf::LineStrip, 1000);
    for (int x = 0; x < 1000; x++) {
		// // float complex ycomp = (cosf(phi - x / 100.0f) + _Complex_I*sinf(phi - x / 100.0f));
		// signal_in  = cexpf(_Complex_I * (phase_in - x / 100.0f));
        // signal_out = cexpf(_Complex_I * (phase_out - x / 100.0f));
		// float y =  crealf(signal_in) * amplitude;
		// float y1 =  crealf(signal_out) * amplitude;
		// line[x] = sf::Vertex(sf::Vector2f(x, y + screen_center_y));
		// line[x].color = sf::Color::Blue;

		// // float complex ycomp1 = cosf(phi_hat ) + _Complex_I*sinf(phi_hat );
		// line1[x] = sf::Vertex(sf::Vector2f(x, y1 + screen_center_y));
		// line1[x].color = sf::Color::White;
		// float complex ycomp = (cosf(phi - x / 100.0f) + _Complex_I*sinf(phi - x / 100.0f));
		float y =  crealf(signal_in[x]) * amplitude;
		float y1 =  crealf(signal_out[x]) * amplitude;
		line[x] = sf::Vertex(sf::Vector2f(x, y + screen_center_y));
		line[x].color = sf::Color::Blue;

		// float complex ycomp1 = cosf(phi_hat ) + _Complex_I*sinf(phi_hat );
		line1[x] = sf::Vertex(sf::Vector2f(x, y1 + screen_center_y));
		line1[x].color = sf::Color::White;
	}
	// float phase_error = cargf( cexpf(_Complex_I * phase_in ) * cexpf(_Complex_I * phase_out) );	
	// phase_out     += alpha * phase_error;    // adjust phase
    // frequency_out +=  beta * phase_error;    // adjust frequency

    // // increment input and output phase values
    // phase_in  += frequency_in;
    // phase_out += frequency_out;
    window.draw(line);
    window.draw(line1);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Moving Sine Wave");
    
	for (int x = 0; x < 1000; x++) {
	signal_in[x]  = cexpf(_Complex_I * phase_in);
	signal_out[x] = cexpf(_Complex_I * (phase_out));

	float phase_error = cargf( cexpf(_Complex_I * phase_in ) * cexpf(_Complex_I * phase_out) );	
	phase_out     += alpha * phase_error;    // adjust phase
    frequency_out +=  beta * phase_error;    // adjust frequency
    // increment input and output phase values
    	phase_in  += frequency_in;
    	phase_out += frequency_out;
	}

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