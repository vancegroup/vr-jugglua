
if(INSTALL_SNAPSHOT_README_TO_ROOT)
	install(FILES BINARY_README
		DESTINATION .)
endif()

if(MARKDOWN_FOUND)
	add_markdown_target(markdown-binary-readme "${CMAKE_CURRENT_BINARY_DIR}" BINARY_README RENAME README.html)
	if(INSTALL_SNAPSHOT_README_TO_ROOT)
		install_markdown_target(markdown-binary-readme
			DESTINATION .
			OPTIONAL)
	else()
		install_markdown_target(markdown-binary-readme
			DESTINATION ${SHARE_DIR}
			OPTIONAL)
	endif()
endif()
