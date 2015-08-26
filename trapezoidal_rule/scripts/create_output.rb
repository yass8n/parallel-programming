#!/usr/bin/ruby
# This script runs the MPI program using different 
# values of n and different numbers of processors
class MPIScript
        def initialize(a, b, n, p)
                @a = a
                @b = b
                @n = n
                @p = p
                @results = []
                @speedup    = 1 #dummy initial value
                @lowest_time = 0 #dummy initial value
                @efficiency = 1 #dummy initial value
        end


        #declaring getters
        def lowest_time
         @lowest_time 
        end
        def speedup
         @speedup
        end
        def efficiency
         @efficiency
        end


        def run(file)
                i = 0
                while i < 5 do
                        run = run_cmd()
                        if run != nil then
                                file.write("          {\n")
                                run.each do |key, value|
                                        file.write("            " + key + " : " + value + "\n")
                                end
                                file.write("          },")
                                file.write("\n")
                                @results.push(run)
                                i += 1
                        end
                        set_lowest_time
                end
        end
        def  calc_speedup_and_efficiency(lowest_timing_for_1p, file)
                @speedup    = lowest_timing_for_1p / @lowest_time
                @efficiency = speedup / @p
        end 
        #runs the code on the commandline and passes in the appropraite values for a, b and n
        def run_cmd
            hostfile = "#{@p.to_s}cores.txt"
            begin
                    cmd = `echo "#{@a} #{@b} #{@n}" | mpirun --prefix /opt/openmpi -hostfile #{hostfile} ./trap_run`
            rescue
                    return nil 
            end 

            #If the previous run failed, try again 
            p = parse_output(cmd)
            if p["time"] == nil then
              puts "failed run, trying again.......\n"
              sleep(2)
              return run_cmd
            else
              return p
            end
        end

        
        #Parses the output to be written to "output.txt"
        def parse_output(cmd)
                cmd = cmd.split("\n")
                result = Hash.new 
            if cmd[0] == "Enter int values for a, b, and n"
                result["time"] = cmd[3]
                result["num_trapezoids"] = cmd[4]
                result["result"] = cmd[5]
                result["abs_true_error"] = cmd[6]
                result["time"].gsub! 'Elapsed time = ', ''
                result["time"].gsub! ' seconds', ''
                result["time"].gsub! 'e+00', ''
                result["num_trapezoids"].gsub!  'With n = ', ""
                result["num_trapezoids"].gsub! ' trapezoid(s),', ""
                result["result"].gsub! "our estimate of the integral from 100.000000 to 600.000000 = ", ""
                result["abs_true_error"].gsub! "absolute relative true error = ", ""
                result["abs_true_error"].gsub! " = 5.000000e-15", ""
                result["num_processors"] = @p.to_s
            end
                return result
        end 
        
        #calculates the lowest time it took for the 5 runs
        def set_lowest_time
                all = [];
                @results.each do |result| 
                        all.push(result["time"].to_f)
                end 
                @lowest_time = all.min
        end 
end 


f = File.open("output.txt", 'w')
num_trapz = 2032143
n_values = [num_trapz, num_trapz/16, num_trapz /8, num_trapz / 4,  num_trapz/2, 
            num_trapz *2, num_trapz*4, num_trapz*8, num_trapz*16]
p_values = [1,2,8,14,20] 
timing_result = 0;
n_values.each { |n| 
        
        f.write("\n\n******** CALCULATED n = #{n} ********=\n\n" ) if (n == num_trapz)
        f.write("\n\n******** n = #{n} ********=\n\n" ) if (n != num_trapz)
        puts("\n\n******** n = #{n} ********=\n\n" )
        timing_result_for_1p = 0;
        p_values.each { |p| 
                puts("\n**** p = #{p} ****\n")
                f.write("\n[\n")
                r = MPIScript.new(100,600,n,p)
                r.run(f)
                timing_result_for_1p = r.lowest_time if p == 1
                r.calc_speedup_and_efficiency(timing_result_for_1p, f)
                f.write("\n          **** p = #{p} , speedup = #{r.speedup} , efficiency = #{r.efficiency} ****\n")
                f.write("\n]\n")
        }   
}

f.close()
