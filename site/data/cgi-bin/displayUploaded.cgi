#!/usr/bin/env python3
import os

upload_dir = os.environ['UPLOAD_DIR']


def generate_empty():
    html = '''
    <html>
    <head>
        <title>Uploaded Files</title>
    </head>
    <body>
        <h1>No Uploaded Files</h1>
    </body>
    </html>
    '''

    return html


def generate_autoindex(directory):
    html = '''
    <html>
    <head>
        <title>Uploaded Files</title>
    </head>
    <body>
        <h1>Uploaded Files</h1>
        <hr>
        <ul>
    '''

    files = os.listdir(directory)
    for file in files:
        file_path = os.path.join(directory, file)
        if os.path.isfile(file_path):
            size = os.path.getsize(file_path)
            html += f'<li>{file} ({size} bytes)</li>'
        elif os.path.isdir(file_path):
            html += f'<li>{file}</li>'

    html += '''
        </ul>
        <hr>
    </body>
    </html>
    '''

    return html


# Generate the autoindex HTML content
if os.path.exists(upload_dir):
    html_content = generate_autoindex(upload_dir)
else:
    html_content = generate_empty()

# Print the HTML content with appropriate headers
print("Content-type: text/html\n")
print(html_content)
