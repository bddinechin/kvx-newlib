#!/usr/bin/ruby

$LOAD_PATH.push('metabuild/lib')
require 'metabuild'
require 'copyrightCheck'
include Metabuild

options = Options.new(
  'compiler' => {
    'type' => 'keywords',
    'keywords' => %i[gcc llvm],
    'default' => 'gcc',
    'help' => 'Which compiler to use'
  },
  'build_type' => {
    'type' => 'keywords',
    'keywords' => %i[Debug Release],
    'default' => 'Debug',
    'help' => 'Build type.'
  },
  'variant' => {
    'type' => 'keywords',
    'keywords' => %i[mbr cos unknown],
    'default' => 'cos',
    'help' => 'Select build variant.'
  },
  'version' => ['unknown', 'Version of the delivered GDB.']
)

arch           = options['target']
workspace      = options['workspace']
newlib_clone   = options['clone']
variant        = options['variant'].to_s
newlib_path    = File.join(workspace, newlib_clone)
build_dir      = File.join(newlib_path, "#{arch}_build_#{variant}")
prefix         = options.fetch('prefix', File.expand_path('none', workspace))
install_prefix = File.join(prefix, 'newlib', 'devimage')
toolroot       = options.fetch('toolroot', File.expand_path('none', workspace))
if arch == 'riscv64'
  config_target = "#{arch}-#{variant}-elf"
else
  config_target = "#{arch}-#{variant}"
end
build_dirs     = []
build_dirs.push build_dir
newlib_scripts_path = __dir__

repo = Git.new(newlib_clone, workspace)

clean           = CleanTarget.new('clean', repo)
build           = ParallelTarget.new("build_#{variant}", repo)
valid           = ParallelTarget.new("valid_#{variant}", repo, depends: [build])
install         = Target.new("install_#{variant}", repo, depends: [valid])
package         = PackageTarget.new("package_#{variant}", repo, depends: [install])
copyright_check = Target.new('copyright_check', repo)

b = Builder.new('newlib', options, [clean, build, valid, install, package, copyright_check])
b.default_targets = [package]
b.logsession = arch

b.target("build_#{variant}") do
  b.logtitle = "Report for newlib build, arch = #{arch} for variant #{variant}"

  build_newlib_sh = File.join(newlib_scripts_path, 'build-newlib.sh')

  %w[configure_newlib build_newlib].each do |step|
    b.run("#{build_newlib_sh} -i #{install_prefix}" \
          " --newlib-dir #{newlib_path}" \
          " --tools newlib --only #{step}" \
          " --compiler #{options['compiler']}" \
          " --build #{build_dir}" \
          " --triplet #{config_target}" \
          " --toolroot=#{toolroot}")
  end
end

b.target('clean') do
  b.logtitle = "Report for newlib clean, arch = #{arch}"
  b.silent("rm -rf #{build_dir}")
end

b.target("install_#{variant}") do
  b.logtitle = "Report for newlib install, arch = #{arch} for variant #{variant}"

  build_newlib_sh = File.join(newlib_scripts_path, 'build-newlib.sh')

  b.run("#{build_newlib_sh} -i #{install_prefix}" \
        " --newlib-dir #{newlib_path}" \
        ' --tools newlib' \
        ' --only install_newlib' \
        " --compiler #{options['compiler']}" \
        " --build #{build_dir}" \
        " --triplet #{config_target}" \
        " --toolroot=#{toolroot}")

  FileUtils.rm_rf("#{install_prefix}/share")

  toolroot = options['compiler'].to_s == 'llvm' ? File.join(toolroot, "#{arch}-llvm") : toolroot
  b.rsync(install_prefix, toolroot)
end

b.target("valid_#{variant}") do
  b.logtitle = "Report for newlib valid, arch = #{arch} for variant #{variant}"
end

b.target("package_#{variant}") do
  b.logtitle = "Report for newlib packaging, arch = #{arch}"
  pkg_prefix_name = options.fetch('pi-prefix-name', "#{arch}-")
  package_description = "#{arch.upcase} newlib #{variant} package.\n" \
                        "This package provides newlib libc MPPA (#{variant})."
  newlib_ptv = get_ptv(arch, variant, options['compiler'].to_s)
  package_name = "#{pkg_prefix_name}newlib-#{newlib_ptv}-dev"

  if options['compiler'] == 'gcc'
    cd install_prefix
    newlib_files = `find ./ -type f -name "*"`.split("\n")
    newlib_files
      .reject { |file| file =~ /kv[34]-[12]/ || file =~ /riscv/ }
      .select { |file| file =~ /\.[ao]/ }
      .each { |file| FileUtils.rm_rf(file) }
  end

  sysroot = options['compiler'].to_s == 'llvm' ? "#{arch}-llvm" : ""
  b.create_package_with_files(
    name: package_name,
    desc: package_description,
    license: 'BSD and MIT and LGPLv2+ and ISC',
    pkg_files: { install_prefix => "#{b.pi_prefix}/#{sysroot}" }
  )
end

b.target('copyright_check') do
  # do nothing here
end

b.launch
