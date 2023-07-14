#!/usr/bin/env python3
import cgi
import sys
import os

# Set the upload directory
# upload_dir = "/Users/larsbonnefoy/projects/19_Webserv/site/data/uploads"
upload_dir = os.environ['UPLOAD_DIR']

# Create an instance of the FieldStorage class to parse the form data
form = cgi.FieldStorage()
print(os.environ, file=sys.stderr)

# Get the uploaded file
file_item = form["file"]

# Check if a file was uploaded
if file_item.filename:
    # Set the path to save the uploaded file
    file_path = os.path.join(upload_dir, os.path.basename(file_item.filename))

    # Save the file
    with open(file_path, "wb") as file:
        file.write(file_item.file.read())

    print("Content-type:text/html\r\n")
    print("<html>")
    print("<head>")
    print("<title>File Upload</title>")
    print("</head>")
    print("<body>")
    print("<h2>File Uploaded Successfully</h2>")
    print("</body>")
    print("</html>")
else:
    print("Content-type:text/html\r\n")
    print("<html>")
    print("<head>")
    print("<title>File Upload</title>")
    print("</head>")
    print("<body>")
    print("<h2>No file was uploaded</h2>")
    print("</body>")
    print("</html>")
