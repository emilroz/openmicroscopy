#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# Copyright (C) 2014 University of Dundee & Open Microscopy Environment.
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

from test.integration.clitest.cli import CLITest
import pytest

subcommands = ['help', 'login', 'logout', 'group',
               'list', 'keepalive', 'clear', 'file']


class TestSessions(CLITest):

    def setup_method(self, method):
        super(TestSessions, self).setup_method(method)
        self.args += ["sessions"]

    def set_login_args(self, user):
        passwd = self.root.getProperty("omero.rootpass")
        host = self.root.getProperty("omero.host")
        port = self.root.getProperty("omero.port")
        self.args = ["sessions", "login", "-w", passwd]
        self.args += ["%s@%s:%s" % (user.omeName.val, host, port)]

    # Help subcommands
    # ========================================================================
    def testHelp(self):
        self.args += ["-h"]
        self.cli.invoke(self.args, strict=True)

    @pytest.mark.parametrize("subcommand", subcommands)
    def testSubcommandHelp(self, subcommand):
        self.args += [subcommand, "-h"]
        self.cli.invoke(self.args, strict=True)

    # Login subcommand
    # ========================================================================
    def testLoginAsRoot(self):
        user = self.new_user()
        self.set_login_args(user)
        self.args += ["--sudo", "root"]
        self.cli.invoke(self.args, strict=True)
        ec = self.cli.controls["sessions"].ctx._event_context
        assert ec.userName == user.omeName.val

    @pytest.mark.xfail(reason="NYI")  # This must be implemented
    def testLoginAsGroupAdmin(self):
        group = self.new_group()
        grp_admin = self.new_user(group=group, admin=True)
        admin = grp_admin.omeName.val
        user = self.new_user(group=group)
        self.set_login_args(user)
        self.args += ["--sudo", admin]
        self.cli.invoke(self.args, strict=True)
        ec = self.cli.controls["sessions"].ctx._event_context
        assert ec.userName == user.omeName.val

    @pytest.mark.parametrize('with_sudo', [True, False])
    @pytest.mark.parametrize('with_group', [True, False])
    def testLoginMultiGroup(self, with_sudo, with_group):
        group1 = self.new_group()
        client, user = self.new_client_and_user(group=group1)
        group2 = self.new_group([user])

        self.set_login_args(user)
        if with_sudo:
            self.args += ["--sudo", "root"]
        if with_group:
            self.args += ["-g", group2.name.val]
        self.cli.invoke(self.args, strict=True)
        ec = self.cli.controls["sessions"].ctx._event_context
        assert ec.userName == user.omeName.val
        if with_group:
            assert ec.groupName == group2.name.val
        else:
            assert ec.groupName == group1.name.val

    # Group subcommand
    # ========================================================================
    def testGroup(self):
        group1 = self.new_group()
        client, user = self.new_client_and_user(group=group1)
        group2 = self.new_group([user])

        self.set_login_args(user)
        self.cli.invoke(self.args, strict=True)
        ec = self.cli.controls["sessions"].ctx._event_context
        assert ec.groupName == group1.name.val

        self.args = ["sessions", "group", group2.name.val]
        self.cli.invoke(self.args, strict=True)
        ec = self.cli.controls["sessions"].ctx._event_context
        assert ec.groupName == group2.name.val
