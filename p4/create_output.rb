#!/usr/bin/ruby
require 'timeout'
n  = 8000
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
def generate_batch_file(n, p, i)
    file_name = "p-#{p}_n-#{n}_run-#{i}.sh"
    File.open("script.sh", 'a') { |f|
	  f.puts "qsub -l select=1:ncpus=#{p} ./" + file_name
	}
    f = File.open(file_name, 'w')

    file_data = "#!/bin/sh\n
	#PBS -N p-#{p}_n-#{n}_run-#{i}\n
	#PBS -j oe\n
	#PBS -l walltime=1:00:00\n
	#PBS -l select=1:ncpus=#{p}\n
	export OMP_NUM_THREADS=#{p}\n
	./parallel_execute #{n} #{p}"

    f.write(file_data)
    f.close()
    File.chmod(0777, file_name);
    return file_name
end
t1=Thread.new{write_to_cmd}
p_values = [1, 2, 5, 10, 20, 30]
     p_values .each do |p|
        5.times do |i|
            batch_file =  generate_batch_file(n, p, i+1)
        end
    end
