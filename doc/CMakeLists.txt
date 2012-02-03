
if(INSTALL_SNAPSHOT_README_TO_ROOT)
	set(SNAPSHOT_README_INSTALL_DIR .)
else()
	set(SNAPSHOT_README_INSTALL_DIR "${SHARE_DIR}")
endif()

if(MARKDOWN_FOUND)
	add_markdown_target(markdown-binary-readme "${CMAKE_CURRENT_BINARY_DIR}" BINARY_README RENAME README.html)
	install_markdown_target(markdown-binary-readme
		DESTINATION "${SNAPSHOT_README_INSTALL_DIR}"
		OPTIONAL)
endif()

if(NOT MARKDOWN_FOUND) # TODO - when else should we install the unprocessed binary readme?
	install(FILES BINARY_README
		DESTINATION "${SNAPSHOT_README_INSTALL_DIR}")
endif()
