bde_prefixed_override(bsltf process_package)
function(bsltf_process_package retPackage)
    process_package_base("" package ${ARGN})

    bde_struct_get_field(testInterface ${package} TEST_INTERFACE_TARGET)
    bde_interface_target_compile_options(
        ${testInterface}
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:
                /bigobj
            >
    )

    bde_return(${package})
endfunction()