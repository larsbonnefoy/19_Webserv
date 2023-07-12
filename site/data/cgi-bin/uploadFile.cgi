#!/usr/bin/env python
import cgi
import os

# Set the upload directory
upload_dir = "/Users/larsbonnefoy/projects/19_Webserv/site/data/uploads"

# Create an instance of the FieldStorage class to parse the form data
form = cgi.FieldStorage()

# Get the uploaded file
file_item = form["file"]

# Check if a file was uploaded
if file_item.filename:
    # Set the path to save the uploaded file
    file_path = os.path.join(upload_dir, os.path.basename(file_item.filename))

    # Save the file
    print("ici???")
    with open(file_path, "wb") as file:
        file.write(file_item.file.read())

    print("Content-type:text/html\r\n\r\n")
    print("<html>")
    print("<head>")
    print("<title>File Upload</title>")
    print("</head>")
    print("<body>")
    print("<h2>File Uploaded Successfully</h2>")
    print("</body>")
    print("</html>")
else:
    print("Content-type:text/html\r\n\r\n")
    print("<html>")
    print("<head>")
    print("<title>File Upload</title>")
    print("</head>")
    print("<body>")
    print("<h2>No file was uploaded</h2>")
    print("</body>")
    print("</html>")
