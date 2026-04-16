idf_component_register(
    SRCS 
        "main.cpp"
        "get_message.cpp"
        "message_frame.cpp"
        "frame_parser.cpp"
        "crc.cpp"  
        "lora_uart.cpp"
        "reception.cpp"
        "transmission.cpp"
        "loop_guard.cpp"
        "central_logic.cpp"
    INCLUDE_DIRS "."
    )
