#!/usr/bin/ruby
#Simple program to parse through output.txt and print only the timing data with 
#arrows around the minimum value
file = File.new("output.txt", "r")
$i = 0;
$arr = []
while (line = file.gets)
	$i = 0 if $i == 5
	if $arr.length == 5 then
		$min = $arr.min
		$min = $min.to_s
		$arr.map! do |num|
			if num.to_s == $min then
			     "-> " + num.to_s + " <-" 
		    else 
		    	num.to_s
		    end
		end
		$arr.each do |num|
			puts num
		end
        $arr.clear
	end
	puts "\n" if $i == 0
	if line.include? "time : " then
		line.gsub! "time :" , ""
		line.gsub! "\n", ""
		$arr.push(line.to_f)
		$i = $i + 1
	end
end
