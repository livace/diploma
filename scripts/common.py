import logging


def setup_logging():
    logging.basicConfig(
        format='%(asctime)s.%(msecs)03d [%(levelname)s] %(message)s',
        datefmt='%Y-%m-%dT%H:%M:%S',
        level=logging.INFO
    )
