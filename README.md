# TrueDelete

a simple tool designed to make a file unrecoverable as much as possible after permanent deletion.
## Security Issues
TrueDelete cannot guarantee complete destruction of data.
Zero overwriting is best effort and cannot guarantee that all data has been destroyed. This is because of:

- SSD wear levelling.
- Journaling and copy on write in the filesystem.
- Snapshots.
- Backups and cloud sync.
- Remapped sectors in physical storage.

To completely sanitise media use the secure erase/sanitise method provided by the manufacturer.
## License
MIT License.
See LICENSE file for full details.
