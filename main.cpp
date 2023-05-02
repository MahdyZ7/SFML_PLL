#include "waves.hpp"


float yreal[N], yreal1[N];
float yimg[N], yimg1[N];
float phase_error[N];
int start = 0;


void DrawSineWave(sf::RenderWindow& window) {
    sf::VertexArray line_real(sf::LineStrip, N);
	sf::VertexArray line_real1(sf::LineStrip, N);
    sf::VertexArray line_img(sf::LineStrip, N);
	sf::VertexArray line_img1(sf::LineStrip, N);
	sf::VertexArray err(sf::LineStrip, N);
	
    // Define the graph axes
    sf::VertexArray axes(sf::Lines, 2);
    {
		axes[0].position = sf::Vector2f(screen_16th, screen_center_y);
		axes[0].color = sf::Color::White;
		axes[1].position = sf::Vector2f(5* screen_16th, screen_center_y);
		axes[1].color = sf::Color::White;
		window.draw(axes);

		axes[0].position = sf::Vector2f(screen_6_16th , screen_center_y);
		axes[1].position = sf::Vector2f(10 * screen_16th, screen_center_y);
		window.draw(axes);

		axes[0].position = sf::Vector2f(screen_11_16th, screen_center_y);
		axes[1].position = sf::Vector2f(15 * screen_16th, screen_center_y);
		window.draw(axes);

		axes[0].position = sf::Vector2f(screen_16th, screen_fourth_y);
		axes[1].position = sf::Vector2f(screen_16th, screen_3_fourth_y);
		window.draw(axes);
		
		axes[0].position = sf::Vector2f(screen_6_16th , screen_fourth_y);
		axes[1].position = sf::Vector2f(screen_6_16th, screen_3_fourth_y);
		window.draw(axes);
		
		axes[0].position = sf::Vector2f(screen_11_16th, screen_fourth_y);
		axes[1].position = sf::Vector2f(screen_11_16th, screen_3_fourth_y);
		window.draw(axes);
	}

	sf::Text text;

	// select the font
	// text.setFont(font); // font is a sf::Font

	// set the string to display
	// text.setString("Hello world");

	// // set the character size
	// text.setCharacterSize(24); // in pixels, not points!

	// // set the color
	// text.setFillColor(sf::Color::Red);


	// // set the text style
	// text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	// text.setPosition(500.f, 500.f);

	// inside the main loop, between window.clear() and window.display()
	window.draw(text);
	
	

    for (int x = 0; x < N ; x++) {

		line_real[x] = sf::Vertex(sf::Vector2f(x + screen_16th, yreal[(x + start) % N] + screen_center_y));
		line_real[x].color = sf::Color::Blue;
		line_img[x] = sf::Vertex(sf::Vector2f(x + screen_6_16th, yimg[(x + start) % N] + screen_center_y));
		line_img[x].color = sf::Color::Blue;

		line_real1[x] = sf::Vertex(sf::Vector2f(x + screen_16th, yreal1[(x + start) % N] + screen_center_y + 3));
		line_real1[x].color = sf::Color::Green;
		line_img1[x] = sf::Vertex(sf::Vector2f(x + screen_6_16th , yimg1[(x + start) % N] + screen_center_y + 3));
		line_img1[x].color = sf::Color::Green;
		
		err[x] = sf::Vertex(sf::Vector2f(x + screen_11_16th, phase_error[(x + start) % N] + screen_center_y));
		err[x].color = sf::Color::Red;
	}

	
    window.draw(line_real);
    window.draw(line_real1);
    window.draw(line_img);
    window.draw(line_img1);
    window.draw(err);


}

int main() {
	
	state_vector state;
	int flag = 0;

	{
		state.phase_offset = -0.1f;  // carrier phase offset
		state.frequency_offset = 0.3f;  // carrier frequency offset

		// Set global parameters for the sine wave
		state.time1 = 0.0;           
		state.amplitude = 150.0;        
		// initialize states
		state.phi     = state.phase_offset;   // input signal's initial phase
		state.phi_hat = +3.0f;           // PLL's initial phase
		state.noise_flag = 0;

	}

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Moving Sine Wave");
	window.setKeyRepeatEnabled(false);
	std::srand(static_cast<unsigned int>(std::time(0)));
	// parameters
    // float        phase_offset     = 0.00f;  // carrier phase offset
    // float        frequency_offset = 0.30f;  // carrier frequency offset
    float        wn               = 0.01f;  // pll bandwidth
    float        zeta             = 0.707f; // pll damping factor
    float        K                = 1000;   // pll loop gain

    // generate loop filter parameters (active PI design)
    float t1 = K/(wn*wn);   // tau_1
    float t2 = 2*zeta/wn;   // tau_2

    // feed-forward coefficients (numerator)
    float b0 = (4*K/t1)*(1.+t2/2.0f);
    float b1 = (8*K/t1);
    float b2 = (4*K/t1)*(1.-t2/2.0f);

    // feed-back coefficients (denominator)
    //    a0 =  1.0  is implied
    float a1 = -2.0f;
    float a2 =  1.0f;

	// filter buffer
    float v0=0.0f, v1=0.0f, v2=0.0f;

    // print filter coefficients (as comments)
    printf("#  b = [b0:%12.8f, b1:%12.8f, b2:%12.8f]\n", b0, b1, b2);
    printf("#  a = [a0:%12.8f, a1:%12.8f, a2:%12.8f]\n", 1., a1, a2);

    

	for (int i=0; i < N; i++) {
        // compute input sinusoid and update phase
        float complex ycomp = cosf(state.phi) + _Complex_I*sinf(state.phi);
		yreal[i] =  crealf(ycomp) * state.amplitude;
		yimg[i] =  cimagf(ycomp) * state.amplitude;
        state.phi += state.frequency_offset;

        // compute PLL output from phase estimate
        float complex ycomp1= cosf(state.phi_hat) + _Complex_I*sinf(state.phi_hat);
		yreal1[i] =  crealf(ycomp1) * state.amplitude;
		yimg1[i] =  cimagf(ycomp1) * state.amplitude;
        // compute error estimate
        float delta_phi = cargf( ycomp * conjf(ycomp1) );
		phase_error[i] = delta_phi;
		phase_error[i] = delta_phi * state.amplitude;

        // // print results to standard output
        // printf("  %6u %12.8f %12.8f %12.8f %12.8f %12.8f\n",
        //     i, crealf(x), cimagf(x), crealf(y), cimagf(y), delta_phi);

        // push result through loop filter, updating phase estimate
        v2 = v1;  // shift center register to upper register
        v1 = v0;  // shift lower register to center register
        v0 = delta_phi - v1*a1 - v2*a2; // compute new lower register

        // compute new output
        state.phi_hat = v0*b0 + v1*b1 + v2*b2;
		state.time1 += 1.0;
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				flag = 1;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				flag = 0;
			else
				key_press(&state);
        }
        window.clear(sf::Color(60, 50, 70));

		if (flag == 1) {
			// compute input sinusoid and update phase
			float phi_noise;
			if (state.noise_flag == 1)
				phi_noise = state.phi + std::rand() % 100 / 10.0f;
			else
				phi_noise = state.phi; // + std::rand() % 100 / 50.0f;
			// float phi_noise = state.phi; // + std::rand() % 100 / 50.0f;
			float complex ycomp = cosf(phi_noise) + _Complex_I*sinf(phi_noise);
			// state.phi += state.frequency_offset;
			state.phi = state.phase_offset + state.time1 * state.frequency_offset;

			// compute PLL output from phase estimate
			float complex ycomp1 = cosf(state.phi_hat) + _Complex_I*sinf(state.phi_hat);

			// compute error estimate
			float delta_phi = cargf( ycomp * conjf(ycomp1) );


			// push result through loop filter, updating phase estimate
			v2 = v1;  // shift center register to upper register
			v1 = v0;  // shift lower register to center register
			v0 = delta_phi - v1*a1 - v2*a2; // compute new lower register

			// compute new output
			state.phi_hat = v0*b0 + v1*b1 + v2*b2;

			yreal[start] =  crealf(ycomp1) * state.amplitude;
			yreal1[start] =  crealf(ycomp1) * state.amplitude;
			yimg[start] =  cimagf(ycomp1) * state.amplitude;
			yimg1[start] =  cimagf(ycomp1) * state.amplitude;
			phase_error[start] = delta_phi * state.amplitude;
			start =  (start + 1) % N; 
			usleep(1000);
			state.time1 += 1.0;
		}
		DrawSineWave(window);
        window.display();
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

// Initialize the K filter
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