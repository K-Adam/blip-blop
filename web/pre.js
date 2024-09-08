// Browser saves live in IndexedDB, main() waits until they are loaded
(Module.preRun ||= []).push(() => {
	FS.mkdir('/config');
	FS.mount(IDBFS, {}, '/config');
	addRunDependency('load-saves');
	FS.syncfs(true, err => {
		if (err) console.error('Cannot load saves', err);
		removeRunDependency('load-saves');
	});
});
