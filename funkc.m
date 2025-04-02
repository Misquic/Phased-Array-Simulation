classdef funkc
    methods (Static)
        function [times, samples] = read_signal()
            all = readmatrix("signal.csv");
            times = all(:,1);
            samples = all(:, 2:end);
        end
        
        function [Nsmp, Fsmp, dx, dy, N, M, data] = read_details()
            jsonText = fileread("details.json");
            data = jsondecode(jsonText);
            Nsmp = data.Nsmp;
            Fsmp = data.Fsmp;
            dx = data.dx;
            dy = data.dy;
            N = data.N;
            M = data.M;
        end

        function [amplitude, sampledFreq, phase] = fourier(sampled_time, samples, bool_show )
            spectrum = fft(samples);
            spectrum( abs(spectrum) < 1e-4 ) = complex( 0 );
            amplitude = abs(spectrum);

            Nsmp = size(samples,1);
            Fsmp = 1/(sampled_time(2) - sampled_time(1));
            Fbase = Fsmp/Nsmp;                     
            amplitude = amplitude/Nsmp*2;        
            amplitude(1) = amplitude(1)/2;
            sampledFreq = 0:Fbase:(Nsmp-1)*Fbase;

            phase = angle( spectrum )./pi;
            phase( abs(phase)< 1e-7) = complex( 0 );

            if bool_show
                figure(), subplot(3,1,1), plot(sampled_time, samples, 'bx--'), title( 'Sygnał w czasie' );
                ylabel( "Amplituda " );
                xlabel("Czas [s]");
                grid on;

                subplot(3,1,2),  stem( sampledFreq, amplitude ), title( 'Widmo amplitudowe' );
                ylabel( "Amplituda " );
                xlim([0 Fsmp/2])
                xlabel("Częstotliwość [Hz]");
                grid on;

                subplot(3, 1, 3), stem( sampledFreq, phase), title( 'Faza' );
                ylabel( "Faza [pi]" );
                xlabel("Częstotliwość [Hz]");
                xlim([0 Fsmp/2])
                grid on;
            end
        end

        function [peaks, indexes] = find_peaks(amplitude)
            peaks = amplitude(amplitude > 1e-4);
            peaks = peaks(1:cast(round(size(peaks,1)/2), "uint32"));
            %disp(amplitude);
            %disp(peaks);
            indexes = find(amplitude > 1e-4);
            indexes = indexes(1:cast(round(size(indexes, 1)/2), "uint32"));
            %disp(indexes);
            
        end

        function signals = signals_separate(ampl, freq, phase, Times)
            [peaks, indexes] = funkc.find_peaks(ampl);
    
            signals = [];
            for i = 1:size(indexes,1)
                sig = peaks(i)*cos(2 * pi *freq(indexes(i)) * Times + phase(indexes(i)) * pi );
                signals = [signals, sig];

            end
            
        end

        function [thetas, phis] = read_angles(data)
           
            A = struct2cell(data.Waves);
            l = length(A);
            thetas = [];
            phis = [];
            for i = 1:l
                B = struct2cell(A{i});
                k = cell2mat(struct2cell(B{2}));
                k = transpose(k);
                thetas = [thetas (90 - funkc.calc_theta_vectors(k, [1 0 0]))];
                phis = [phis (90 -funkc.calc_theta_vectors(k, [0 1 0]))];
            end
        end

        function [peak, index] = find_max_peak(amplitude)
            [peak, index] = max(amplitude);
        end

        function [phase_diff, theta] = calc_theta(samples, bool_show, freq, dd)
            if(size(samples,2)>size(samples,1))
                samples = transpose(samples);
            end
            %%%zeropadding
            Nsmp = size(samples, 1);
            if iscolumn(samples)  % Jeśli próbki są w kolumnie
                samples = [zeros([1000, 1]); samples; zeros([1000-Nsmp, 1])];  % Dodaj zerowe wiersze
            else  % Jeśli próbki są w wierszu
                samples = [zeros([1, 1000]), samples, zeros([1, 1000-Nsmp])];  % Dodaj zerowe kolumny
            end
            Nsmp = size(samples, 1);

            times = linspace(0,Nsmp-1, Nsmp);
            %%%%%%%%%%%%
            [ampl_space, freq_space, phase_space] = funkc.fourier(times, samples, bool_show);
            [peak, index] = funkc.find_max_peak(ampl_space); % co jeśli kilka?
            fsig = freq_space(index)
            
            %freq_space(index)
            theta = asind(omega * c / fsig/2/pi/dd);

            % figure(),  stem( freq_space, ampl_space ), title( 'Widmo amplitudowe' );
            % title(freq_space(index));
            % ylabel( "Amplituda " );
            % xlim([0 1])
            % xlabel("sin(theta)");
            % grid on;








            % %%%%%%%%%%%
            % %index
            % c = 299792458.0;
            phase_diff = phase_space(index)*pi;
            % theta = acosd(phase_diff * c / freq / (2 * pi) / dd) - 90; % Obliczenie kąta padania w radianach
            % %fprintf('Obliczony kąt: %.2f stopni\n', theta);
        end

        function theta = calc_theta_vectors(vec1, vec2)
            theta = acosd(sum(vec1.*vec2)/funkc.vec_len(vec1)/funkc.vec_len(vec2));
        end

        function len = vec_len(vec)
            len = sum(vec.*vec);
            len = sqrt(len);
        end

        
        function [phase_diff, theta] = calculate_phase_difference(Times, signal1, signal2, Fsig, dd)
            

            [ampl1, freq1, phase1] = funkc.fourier(Times, signal1, true);
            [ampl2, freq2, phase2] = funkc.fourier(Times, signal2, true);

            [peak, index] = funkc.find_max_peak(ampl1);

            phase_diff = (phase2(index) - phase1(index))*pi;
            c = 299792458.0;
            theta =  acosd(phase_diff* c/Fsig/2/pi/dd) - 90;
            theta_rad = deg2rad(theta);


        end
        

      
    end
end