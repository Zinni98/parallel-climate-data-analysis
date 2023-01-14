"Parsing command line secifics algorithm"
import argparse
from typing import List


class Parser:
    """Class thath implements a command line parser"""

    def __init__(self) -> None:
        self.parser = argparse.ArgumentParser(
            # program name
            prog="Benchmark plotter",
            # brief description of what the algorithm does.
            # Displayed before usage
            description="Algorithm to print benchmark trends",
            # no default values
            argument_default=None,
            add_help=True,
            # allow to use -inital letter
            allow_abbrev=True,
            # exit with error info in case of invalid options
            exit_on_error=True,
        )

    def add_arg(
        self, name: str, choices: List, custom_action: str = "store"
    ) -> None:
        """Function to add argument to the parser
        Parameters:
        ------------
            name : str
                name of the flag to add
        """
        self.parser.add_argument(
            "--" + name,
            "-" + name,
            type=type(choices[0]),
            action=custom_action,
            choices=choices,
            default="Time",
            dest=name,
        )

    def parse_args(self):
        """Function that build a new Namespace class for each parse_args"""
        # by default parses all arguments
        args = self.parser.parse_args(args=None, namespace=None)
        return args
