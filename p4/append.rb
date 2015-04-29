#!/usr/bin/ruby
Dir.foreach("/Users/yass8n/CSCI/551_Parallel_pro/csci551-master/p4") do |fname|
	if fname.include? "o3"
		File.open("output.txt", 'a') { |f|
		  `cat #{fname} >> output.txt`
		}
	end
end