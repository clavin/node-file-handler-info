export interface FileHandlerInfo {
	handlerPath: string | null;
	friendlyName: string | null;
}

export function getHandlerInfo(filePath: string): FileHandlerInfo;
