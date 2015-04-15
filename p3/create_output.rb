#!/usr/bin/ruby
# This script runs the MPI program using different 
# values of n and different numbers of processors
require 'timeout'

def write_to_cmd
  begin
    result = Timeout::timeout(2) do
      sleep(1)
    end
    puts "The result was #{result}"
    write_to_cmd
  rescue Timeout::Error
    puts "the calculation timed out"
    write_to_cmd
  end
end
class MPIScript
        def initialize(n, p, form)
                @n = n
                @p = p
                @form = form
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
                        puts "..........Running"
                        run = run_cmd()
                        if run != nil then
                                puts "..........Ran\n\n\n\n\n\n"
                                file.write("          {\n")
                                run.each do |key, value|
                                        file.write("            " + key + " : " + value + "\n")
                                end
                                file.write("          },")
                                file.write("\n")
                                @results.push(run)
                                i+=1
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
                    cmd = `echo "#{@form} R #{@n}" | mpirun --prefix /opt/openmpi -hostfile #{hostfile} ./parallel_mult`
            rescue
                    return nil 
            end 

            # If the previous run failed, try again 
            p = parse_output(cmd)
            if p["time"] == nil then
              puts "failed run, trying again.......\n"
              sleep(2)
              return run_cmd
            else
              puts p  
              return p
            end
        end

        
        #Parses the output to be written to "output.txt"
        def parse_output(cmd)
                cmd = cmd.split("\n")
                result = Hash.new 
            if cmd[0] == "Enter data: "
                result["num_processors"] = cmd[1]
                result["num_processors"].gsub! 'running on ', ''
                result["num_processors"].gsub! ' processor(s) ', ''
                result["time"] = cmd[2]
                result["time"].gsub! 'Elapsed time = ', ''
                result["time"].gsub! ' seconds', ''
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
t1=Thread.new{write_to_cmd}
p_values = [1,4,8,12,16,20] 
ijk_values = ["ijk", "ikj","kij"] 
timing_result = 0;
    ijk_values.each { |form|
        puts("\n\n******** form = #{form} ********=\n\n" )
        timing_result_for_1p = 0;
        p_values.each { |p| 
            f.write("\n\n******** NUM PROCESSORS = #{p} ********=\n\n" ) 
            f.write("\n[\n")
            r = MPIScript.new(4800,p,form)
            r.run(f)
            timing_result_for_1p = r.lowest_time if p == 1
            r.calc_speedup_and_efficiency(timing_result_for_1p, f)
            f.write("\n          **** p = #{p} , speedup = #{r.speedup} , efficiency = #{r.efficiency} ****\n")
            f.write("\n]\n")
        }
    }

f.close()
