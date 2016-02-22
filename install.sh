echo 'Installing Data Audit...'

mkdir -p /usr/local/data_audit/files_dir
mkdir -p /usr/local/data_audit/file_tags
mkdir -p /usr/local/data_audit/block_tags
mkdir -p /usr/local/data_audit/dropboxtags
mkdir -p /usr/local/data_audit/dropboxfiles
mkdir -p /usr/local/data_audit/temp_tags_files

bin/install_main

echo 'Done'
