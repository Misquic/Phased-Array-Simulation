clear; clc; close all;

[Times, All_samples] = funkc.read_signal();

[Nsmp, Fsmp, dx, dy, N, M, data] = funkc.read_details();
c = 299792458.0;

[thetas_simulated, phis_simulated] = funkc.read_angles(data); % theta w osi x, phi w osi y

samples_0_0 = All_samples(:, 1);
samples_0_1 = All_samples(:, 2);

[amplitude_0_0, freq_0_0, phase_0_0] = funkc.fourier(Times, samples_0_0, false);
signals_separated = funkc.signals_separate(amplitude_0_0, freq_0_0, phase_0_0, Times);
[peaks, indexes] = funkc.find_peaks(amplitude_0_0);

[peak, index] = funkc.find_max_peak(amplitude_0_0);

Nsig = size(indexes,1); % liczba sygnałow o różnych częstotliwościach

all_signal_separated = [];
thetas_calculated = [];
phis_calculated = [];

all_signal_separated{size(All_samples, 2)} = signals_separated;
for i = 1:size(All_samples, 2)
    %disp(i);
    samples = All_samples(:, i);
    [ampl, freq, phase] = funkc.fourier(Times, samples, false);
    signals_separated = funkc.signals_separate(ampl, freq, phase, Times);
    %disp(signals_separated)
    all_signal_separated{i} = signals_separated;
end

%dla chwili czasowej 0 time index
time_index = 5;
samples_time0 = [];
samples_time0(N*M, Nsig) = 0;
for i = 1:N*M
    for j = 1:Nsig
        A = cell2mat(all_signal_separated(1, i));
        samples_time0(i,j) = A(time_index, j);
    end
end
%disp(samples_time0);

%thetas_simulated

disp("algorytm 1")

g = 0;
for i = 1:Nsig
    thetas_calculated = [];
    phis_calculated = [];
    A = cell2mat(all_signal_separated(1,3));
    samplesx1 = A(:,i);
    freq(indexes(i));
    A = cell2mat(all_signal_separated(1,4));
    samplesx2 = A(:,i);


    [phase_diff, theta] = funkc.calculate_phase_difference(Times, samplesx1, samplesx2, freq(indexes(i)), dx);
    thetas_calculated = [thetas_calculated theta]; % co jeśli kilka

    A = cell2mat(all_signal_separated(1,1));
    samplesy2 = A(:,i);
    freq(indexes(i));
    A = cell2mat(all_signal_separated(1,M+1));
    samplesy1 = A(:,i);
    [phase_diff, phi] = funkc.calculate_phase_difference(Times, samplesy1, samplesy2, freq(indexes(i)), dy);
    phis_calculated = [phis_calculated phi]; % co jeśli kilka
    for j = 1:size(thetas_calculated,2)
        g = g + 1;
        fprintf("czestotliwosc: %.2f, theta_calc: %.2f, theta_sim: %.2f, phi_calc: %.2f, phi_sim: %.2f \n", freq(indexes(i)), thetas_calculated(j), thetas_simulated(g), phis_calculated(j), phis_simulated(g))
    end
    for j = 1:size(thetas_calculated,2)
        fprintf("maksymalny kąt padania theta_max: %.2f\n rozdzielczosc kąta theta: %.2f deg\nmaksymalny kąt padania phi_max: %.2f\n rozdzielczosc kąta phi: %.2f deg\n", asind(c/freq(indexes(i))/2/dx), c/freq(indexes(i))/dx/M, asind(c/freq(indexes(i))/2/dy), c/freq(indexes(i))/dy/N ) 
    end
end




disp(" ")
disp("algorytm 2")

g = 0;
for i =1:Nsig
    
    thetas_calculated = [];
    phis_calculated = [];
    thetas = [];
    phis = [];
    samples = reshape(samples_time0(:,i), [M,N]);
    samples = transpose(samples);
    %disp(samples)
    %all_to_test = all_to_test + samples;
    bool_show = false;
    for n = 1:N
        if n == 1
            bool_show = false; %zmienic na true jesli sie chce zobaczyc wykresy
        else
            bool_show = false;
        end
        %disp(funkc.calc_theta(samples(n,:), bool_show))
        [phase, theta] = funkc.calc_theta(samples(n,:), bool_show, freq(indexes(i)), dx);
        thetas = [thetas theta]; % co jeśli kilka
    end
    thetas_calculated = [thetas_calculated mean(thetas,2)];

    
    for m = 1:M
        if m == 4
            bool_show = false; %zmienic na true jesli sie chce zobaczyc wykresy
        else
            bool_show = false;
        end
        %disp(funkc.calc_theta(samples(:,m), bool_show))
        [phase, phi] = funkc.calc_theta(flip(samples(:,m)), bool_show, freq(indexes(i)), dy); % obrócic
        phis = [phis phi]; % co jeśli kilka
    end
    phis_calculated = [phis_calculated mean(phis,2)];

    for j = 1:size(thetas_calculated,2)
        g = g + 1;
        fprintf("czestotliwosc: %.2f, theta_calc: %.2f, theta_sim: %.2f, phi_calc: %.2f, phi_sim: %.2f \n", freq(indexes(i)), thetas_calculated(j), thetas_simulated(g), phis_calculated(j), phis_simulated(g))
    end
    for j = 1:size(thetas_calculated,2)
        fprintf("maksymalny kąt padania theta_max: %.2f\n rozdzielczosc kąta theta: %.2f deg\nmaksymalny kąt padania phi_max: %.2f\n rozdzielczosc kąta phi: %.2f deg\n", asind(c/freq(indexes(i))/2/dx), c/freq(indexes(i))/dx/M, asind(c/freq(indexes(i))/2/dy), c/freq(indexes(i))/dy/N ) 
    end
end


% Nsmp = 50;
% times = linspace(1,Nsmp, Nsmp)/5e9;
% c = 299792458.0;
% fsig = 1e9;
% theta_teor = 30
% theta_rad = deg2rad(theta_teor); % Konwersja kąta na radiany
% dd_effective = dx * cos(theta_rad); % Obliczenie efektywnej odległości w kierunku padania
% phase_teor = (2 * pi * dd_effective) / c * fsig; % Obliczenie opóźnienia fazowego

% dd_calculated = (phase_teor * c / fsig) / (2 * pi); % Obliczenie odległości w kierunku padania
% theta_calculated_rad = acos(dd_calculated / dx); % Obliczenie kąta padania w radianach
% theta_calculated_deg = rad2deg(theta_calculated_rad); % Konwersja kąta na stopnie
% % Wyświetlenie obliczonego kąta padania
% fprintf('Obliczony kąt padania: %.2f stopni\n', theta_calculated_deg);

% %% k_x * x + k_y =0 * y + k_z * z = 0
% phase_teor = 14.82 * dx %rad
% fsig = 1e9;
% smp1 = cos(Times*2*pi*fsig );
% smp2 = cos(Times*2*pi*fsig  + phase_teor);

% [phase_diff, theta] = funkc.calculate_phase_difference(Times, smp1, smp2, fsig, dx);
% [phase_diff, theta] = funkc.calc_theta(smp2, false, fsig, dx);
% %phase_diff
% %theta

