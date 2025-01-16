# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -g -std=c++17 -I/usr/local/include # Include xlsxwriter header
TARGET = main
SRC = main.cc models.cc graph.cc utils.cc directoryManager.cc UnionFind.cc excel_image_generator.cc # Sources of the project

# Find all directories that end with _dot_file
DOT_DIRS = $(shell find . -type d -name '*_dot_file')

# Find all .dot files in the directories
DOT_FILES = $(foreach dir,$(DOT_DIRS),$(wildcard $(dir)/*.dot))

# Define PDF output filenames in the output_pdfs directory
PDF_FILES = $(foreach dot_file, $(DOT_FILES), output_pdfs/$(notdir $(dot_file:.dot=.pdf)))

# Compiling the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lxlsxwriter # Link with xlsxwriter

# Rule to generate PDFs from DOT files
output_pdfs/%.pdf: $(DOT_DIRS)/%.dot
	@mkdir -p output_pdfs  # Create the output directory for PDFs if it doesn't exist
	dot -Tpdf $< -o $@  # Convert the .dot file to .pdf

# Rule to generate all PDFs
pdfs: $(PDF_FILES)

# Clean generated files
clean:
	rm -f $(TARGET)
	rm -rf output_pdfs
	rm -rf */  # Remove all directories
