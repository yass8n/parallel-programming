#!/usr/bin/ruby
def display(result_array)
        print "  #{result_array[1]}:" #line number
        for i in 2..result_array.length
                print result_array[i] #contents in that line 
        end
    print "\n"
end
def print_file_name(file_name, files_array)
        puts "--------------------------------------------------" unless files_array.empty? #don't print if this is the first file 
        puts "./#{file_name}" #file name
        files_array = files_array.push(file_name) #modifying the actual files_array and not just the local copy in this function
end

search_expression = ARGV[0];
files_array = []
extension_array = []
file_extensions = ARGV[1]
file_extensions = "*" if file_extensions.nil? || file_extensions.empty?
grep_result = ( if file_extensions == "*" then `grep -rinw * -e #{search_expression}` else `grep --include=\*.{#{file_extensions},#{file_extensions}} -rinw * -e #{search_expression}` end )

#initializing valid file extensions
file_extensions_split = file_extensions.split(",")
file_extensions_split.each { |ext|
        extension_array << ext
}

#printing the valid and matching file paths
puts "Files with names that matches <#{search_expression}>"
valid_paths = `find * -iname "*#{search_expression}*"`
valid_paths = valid_paths.split("\n")
valid_paths.each { |path|
        split_path = path.split('.')
        extension = split_path.last.to_s
         puts "  ./#{path}"  if ( file_extensions == "*" || (extension_array.include? extension) )#if the file name has the correct extension, print it to the screen
} unless valid_paths.empty?

#printing the matching content
puts "**************************************************"
puts "Files with content that matches <#{search_expression}>"
grep_result = grep_result.split("\n")
grep_result.each { |result|
        result_array = result.split(":")
        print_file_name(result_array[0], files_array) unless files_array.include? result_array[0]
    display(result_array) #displays contents of the file 
} unless grep_result.empty?