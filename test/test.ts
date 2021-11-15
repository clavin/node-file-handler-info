/*
 * README:
 *
 * This test file makes a lot of assumptions about the environment it is running
 * in. Particularly:
 *
 * 1. It is expected that the test machine still has the default handlers for
 *    extremely common file types (.txt, .mp4, etc.).
 * 2. It is expected that the test machine is set to U.S. American English.
 *
 * If any of those invariants don't hold then it's not guaranteed these tests
 * will pass, even if there is no issue.
 */

import { getHandlerInfo, FileHandlerInfo } from '..';
import path = require('path');

const FIXTURES_PATH = path.join(__dirname, 'fixtures');

const NO_INFO: FileHandlerInfo = {
  handlerPath: null,
  friendlyName: null,
};

/**
 * Selects the key from the mapping that matches the current platform or a
 * fallback value if the current platform is not in the mapping.
 */
function selectPlatformKey<T>(
  mapping: Partial<Record<(typeof process)['platform'], T>>,
  fallback: T,
): T {
  return mapping.hasOwnProperty(process.platform)
    ? mapping[process.platform]
    : fallback;
}

test('bad path strings', () => {
  // hopefully do not crash.
  expect(getHandlerInfo('')).toEqual(NO_INFO);
  expect(getHandlerInfo('this path doesnt exist!')).toEqual(NO_INFO);
  expect(getHandlerInfo('*!#&*!#%&*(|)!(\\;:/><>:[]}{')).toEqual(NO_INFO);
});

test('.mp4 file', () => {
  expect(
    getHandlerInfo(path.join(FIXTURES_PATH, 'empty.mp4'))
  ).toEqual(selectPlatformKey(
    {
      'win32': {
        handlerPath: 'C:\\Program Files (x86)\\Windows Media Player\\wmplayer.exe',
        friendlyName: 'Windows Media Player',
      },
      'darwin': {
        handlerPath: '/System/Applications/QuickTime Player.app',
        friendlyName: 'QuickTime Player',
      },
    },
    NO_INFO,
  ));
});

test('.txt file', () => {
  expect(
    getHandlerInfo(path.join(FIXTURES_PATH, 'empty.txt'))
  ).toEqual(selectPlatformKey(
    {
      'win32': {
        handlerPath: 'C:\\Windows\\system32\\NOTEPAD.EXE',
        friendlyName: 'notepad',
      },
      'darwin': {
        handlerPath: '/System/Applications/TextEdit.app',
        friendlyName: 'TextEdit',
      },
    },
    NO_INFO,
  ));
});
