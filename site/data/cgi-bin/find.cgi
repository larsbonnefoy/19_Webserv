#!/usr/bin/env python3
import os
import sys

upload_dir = os.environ['UPLOAD_DIR']

print(upload_dir, file=sys. stderr)


def generate_autoindex(directory):
    html = '''
    <html>
    <head>
        <title>Index of {directory}</title>
    </head>
    <body>
        <h1>Index of {directory}</h1>
        <hr>
        <ul>
    '''

    files = os.listdir(directory)
    for file in files:
        file_path = os.path.join(directory, file)
        if os.path.isfile(file_path):
            size = os.path.getsize(file_path)
            html += f'<li><a href="{file}">{file}</a> ({size} bytes)</li>'
        elif os.path.isdir(file_path):
            html += f'<li><a href="{file}/">{file}/</a></li>'

    html += '''
        </ul>
        <hr>
    </body>
    </html>
    '''

    return html


# Generate the autoindex HTML content
html_content = generate_autoindex(upload_dir)

# Print the HTML content with appropriate headers
print("Content-type: text/html\n")
print(html_content)
