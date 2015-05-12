#!/usr/bin/ruby
Dir.foreach("/Users/yass8n/CSCI/551_Parallel_pro/csci551-master/p4/output") do |fname|
    if fname.include? "o6"
        File.open("output.txt", 'a') { |f|
          `cat #{fname} >> output.txt`
        }
    end
end