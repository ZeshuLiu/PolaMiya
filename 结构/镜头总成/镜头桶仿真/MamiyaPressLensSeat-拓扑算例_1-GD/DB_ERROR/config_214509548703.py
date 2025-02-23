#!/usr/bin/python
# -*- coding: utf_8 -*-
import os, os.path, sys, shutil
from datetime import datetime
from SMATsoEnum import *
from SMATsoLogging import *
from SMATsoModule import *
from SMATsoFemInput import *
from SMATsoSolver import *
from SMATsoSolverAbaqus import *
from SMATsoSolverAnsys import *
from SMATsoSolverMSCNastran import *
from SMATsoSolverOnf import *
from SMATsoSolverFeSafe import *
from SMATsoSolverFemfat import *
from SMATsoUtilPar import scanParFile
from SMATsoConfig import *


def config_214509548703_0(config, driver, installer_config):
    source_file = r'E:\Software\Solidworks2023\Solidworks\SOLIDWORKS\Simulation\Topology\SMATsoResources\config\tosca.cfg'

    import os
    
    # installer_config: Settings from installer (<TOSCA_INSTALL_DIR>/<osds>/SMA/site/EstablishedProductsConfig.ini)
    
    isWin = driver.Platform == Platforms.WINDOWS
    isLin = driver.Platform == Platforms.LINUX
    
    # ----------------------------------- #
    # General File archiving registration #
    # ----------------------------------- #
    
    # Copy par file and files mentioned in INCLUDE commands to working directory
    files = [ driver.ParFile ] + scanParFile( driver.ParFile, [ r'INCLUDE' ] ).get( r'INCLUDE', [] )
    driver.registerSaveRule( UpdateRules.COPY,
                             CheckPoints.OPT_BEGIN,
                             EventTimes.EVER,
                             files,
                             save_dir = r'' )
    
    # Copy original solver files mentioned in INCLUDE commands to working directory
    if driver.FemInput.AllFiles:
        driver.registerSaveRule( UpdateRules.COPY,
                                 CheckPoints.OPT_COMPLETE,
                                 EventTimes.EVER,
                                 driver.FemInput.AllFiles,
                                 save_dir = r'' )
    
    driver.registerSaveRule( UpdateRules.COPY,
                             CheckPoints.PREP_COMPLETE,
                             EventTimes.EVER,
                             [ r'MODEL_fem.onf',
                               r'__FE_MODEL___fem.onf',
                               r'__FE_MODEL___grp.onf',
                               r'GROUPS.onf',
                               r'GROUPS_PREP.onf' ],
                             save_dir = r'SAVE.onf' )
    
    driver.registerSaveRule( UpdateRules.DELETE,
                             CheckPoints.OPT_COMPLETE,
                             EventTimes.EVER,
                             [ r'MODEL_fem.onf',
                               r'__FE_MODEL___fem.onf',
                               r'__FE_MODEL___grp.onf',
                               r'GROUPS.onf',
                               r'GROUPS_PREP.onf' ],
                             save_dir = r'SAVE.onf' )
    
    # move core input ONF files to SAVE.onf upon completion of the last iteration
    #driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.OPT_COMPLETE, EventTimes.LAST,
    #    [ r'__FE_MODEL___fem.onf', r'__FE_MODEL___grp.onf', , r'MODEL_fem.onf', r'GROUPS.onf', r'GROUPS_PREP.onf', r'*_GROUPS.onf', r'sect_parent2child.onf', r'TOSCA_MORPH_[0-9][0-9][0-9].onf' ],
    #    r'SAVE.onf' )
    
    # move result ONF files to SAVE.onf upon completion of every iteration
    driver.registerSaveRule( UpdateRules.MOVE,
                             CheckPoints.CYCLE_COMPLETE,
                             EventTimes.EVER,
                             [ r'SHAPE_[0-9][0-9][0-9].onf',
                               r'SHAPE_SENS_[0-9][0-9][0-9].onf',
                               r'TP_[0-9][0-9][0-9].onf',
                               r'TP_SENS_[0-9][0-9][0-9].onf',
                               r'SIZING_[0-9][0-9][0-9].onf',
                               r'SIZING_SENS_[0-9][0-9][0-9].onf',
                               r'BEAD_[0-9][0-9][0-9].onf',
                               r'BEAD_SENS_[0-9][0-9][0-9].onf',
                               r'MODEL_???.onf',
                               r'MODEL_FEM.onf',
                               r'GROUPS_???.onf',
                               r'*_GROUPS.onf',
                               r'TEST_SHAPE_*.onf',
                               r'TEST_BEAD_*.onf',
                               r'TOSCA_MORPH_[0-9][0-9][0-9].onf' ],
                             save_dir = r'SAVE.onf' )
    
    # move result files to TOSCA_POST upon completion of the optimization job
    driver.registerSaveRule( UpdateRules.MOVE,
                             CheckPoints.OPT_COMPLETE,
                             EventTimes.EVER,
                             [ r'MamiyaPressLensSeat-拓扑算例_1-GD_post.par',
                               r'MamiyaPressLensSeat-拓扑算例_1-GD_post_[0-9][0-9][0-9].par',
                               r'tosca_structure_report.par',
                               r'*.conf',
                               r'*.tab' ],
                             save_dir = r'TOSCA_POST' )
    
    # delete temporary files of Tosca
    driver.registerSaveRule( UpdateRules.DELETE,
                             CheckPoints.OPT_COMPLETE,
                             EventTimes.EVER,
                             [ r'TOSCA_OK.DAT',
                               r'design_cycle.cfg',
                               r'*.vtm',
                               r'*.idx',
                               r'__FE_MODEL___lc.onf',
                               r'__FE_MODEL___mod.onf',
                               r'__FE_MODEL___req.onf',
                               r'__FE_MODEL___init.onf',
                               r'STOP_DAT.CAO',
                               r'CLOAD_INFORMATION.INP',
                               r'tosca_distribution.bin',
                               r'tosca_distribution.txt',
                               r'TOSCA_MORPH_mod_[0-9][0-9][0-9].onf',
                               r'*.0*',
                               r'*.solMap.*',
                               r'*.pre*',
                               r'*.symbFct',
                               r'*_TS.txt',
                               r'*.map',
                               r'*_TS.bin*',
                               r'restart.py*'] )
    
    driver.registerSaveRule( UpdateRules.DELETE,
                             CheckPoints.OPT_COMPLETE,
                             EventTimes.NEVER,
                             [ r'opt_res_database.vtm',
                               r'opt_res_[0-9][0-9][0-9].vtm',
                               r'opt_res_database.idx',
                               r'resume_database.vtm',
                               r'resume_database.idx',] )
    
    # copy ONF result files from DB_ERROR folder to SAVE.onf folder in case of an exception
    # This enables Tosca_report to find all relevant ONF files in the expected folder and display them e.g. in the VTFX file.
    driver.registerSaveRule( UpdateRules.COPY,
                             CheckPoints.OPT_EXCEPTION,
                             EventTimes.EVER,
                             [ os.path.abspath( os.path.join(driver.WorkDir, r'DB_ERROR', f) ) for f in
                                [ r'SHAPE_[0-9][0-9][0-9].onf', r'SHAPE_SENS_[0-9][0-9][0-9].onf', r'TP_[0-9][0-9][0-9].onf', r'TP_SENS_[0-9][0-9][0-9].onf',
                                  r'SIZING_[0-9][0-9][0-9].onf', r'SIZING_SENS_[0-9][0-9][0-9].onf', r'BEAD_[0-9][0-9][0-9].onf', r'BEAD_SENS_[0-9][0-9][0-9].onf',
                                  r'MODEL_???.onf', r'MODEL_fem.onf', r'GROUPS_???.onf', r'GROUPS.onf',
                                  r'TEST_SHAPE_*.onf', r'TEST_BEAD_*.onf', r'TOSCA_MORPH_[0-9][0-9][0-9].onf' ] ],
                                  r'SAVE.onf' )
    
    # register various additional files
    driver.registerAdditionalFiles()
    
    # ------------------------ #
    # FE-Solver Configurations #
    # ------------------------ #
    
    solver = driver.Solver
    if solver:
    
        # Upon completion of a design cycle save modified input files.
        # In case of morphing a directory with (-01) is created.
        # This condition avoids the problem.
        if not driver.OptimizationType in { OptimizationTypes.MORPHING_SHAPE, OptimizationTypes.MORPHING_BEAD }:
            save_dir   = r'SAVE.%s'%solver.inputExt
            save_files = [ os.path.basename( f ) for f in driver.FemInput.solverInputFiles ]
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.CYCLE_COMPLETE, EventTimes.EVER, save_files, save_dir, r'_%i', True )
    
        # ------------ #
        # -- ABAQUS -- #
        # ------------ #
        if solver.Name == FeSolvers.ABAQUS:
            solver.Path = (  driver.FemInput.SolverPath
                          or os.environ.get( r'SMA_TSO_ABAQUS_PATH' )
                          or installer_config[ InstallConfig.INS_TSO_ABAQUS_PATH ]
                          or ( r'SMALauncher.exe' if isWin else r'SMALauncher' ) )
            solver.MessageExt       = r'msg'
            solver.StandardCallArgs = driver.FemInput.SolverCmdLine or [ r'-unified', r'-job', r'__FE_MODEL__', r'analysis', r'interactive', r'-resultsFormat', r'ODB' ]
            solver.AddCallArgs      = []
            solver.KillArgs         = [ r'-job', r'__FE_MODEL__', r'terminate' ]
            solver.CheckStrings     = [ r'THE ANALYSIS HAS BEEN COMPLETED', r'' ]
    
            # -- Environment -- #
    
            os.environ[ r'ABA_ALLOW_NFORC' ]            = r'1'
            os.environ[ r'ABA_NO_SITE_JOB_COMPLETION' ] = r'1'
            os.environ[ r'ABQ_DIST_BEAM_RADIUS' ]       = r'1'
            os.environ[ r'ABA_STATICCOND_C3D10M' ]      = r'OFF'
            # SOLVER=ITERATIVE: Use FSAI-smoother for AMG because Tosca may produce implicit regions and so ill-conditioned problems.
            os.environ[ r'ABA_PAMG_SMOOTHER_TYPE' ]     = r'fsai'
            # SOLVER=ITERATIVE: For improved performance and less accuracy uncomment next line. Default is ABA_EQSITR_RTOL=1e-6
            # os.environ[r'ABA_EQSITR_RTOL']=r'1e-3'
    
            if solver.runMode == AbaqusRunModes.SEQUENTIAL:
                # If MAC matrix should be calculated then Abaqus should know if it is running in sequential mode.
                os.environ[ r'ABATOS_COLDMODE' ] =r'1'
            else:
                os.environ[ r'ABATOS_COLDMODE' ] =r'0'
    
            # -- Hooks -- #
            if not solver.RemoteExecInfo:
                abaqus_ts_provider = r'SMATsoAbaqusTsProvider'
                if isWin:
                    abaqus_ts_provider =  abaqus_ts_provider + r'.exe'
    
                parfile_name = os.path.splitext( driver.ParFile )
                driver.registerModuleHook( FeSolvers.ABAQUS, HookTypes.PRE, EventTimes.EVER, [abaqus_ts_provider ,r'-j',r'__FE_MODEL__',r'-p',os.path.join(driver.WorkDir,parfile_name[0])], False, True, False )
                driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.ITER_COMPLETE, EventTimes.EVER, [ r'*.ts' ] )
    
            # -- SaveRules -- #
    
            # remove old *MACZERO*.sim and *MACPREV*.sim files
            if solver.runMode == AbaqusRunModes.SEQUENTIAL:
                driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.OPT_BEGIN, EventTimes.EVER, [ r'*MACZERO*.sim', r'*MACPREV*.sim' ] )
    
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.OPT_COMPLETE, EventTimes.EVER, [ r'*MACZERO*.sim', r'*MACPREV*.sim', r'*.SMABulk', r'*_MAC.onf', r'*.pre'] )
            driver.registerSaveRule( UpdateRules.COPY, CheckPoints.PERT_COMPLETE, EventTimes.FIRST_LAST, [ r'*.pre' ], r'SAVE.pre', r'_%i_%p', bundle = True )
    
            # remove intial extension from job files
            job_names = [ SMATsoUtilFile.setExtension( j, r'' ) for j in solver.JobFiles ]
            if solver.TemperatureFile:
                job_names.append( SMATsoUtilFile.setExtension(solver.TemperatureFile[ 0 ], r'' ) )
    
            out_files  = [ r'uma_diag.log' ]
            extensions = [ r'.fil', r'.dat', r'.prt', r'.sta', r'.msg', r'.msg.*', r'.size', r'.mdl', r'.stt', r'.stall',
                           r'.ipm', r'.lck', r'.cid', r'.023', r'_sfd.tmp', r'.abq', r'.pac', r'.res', r'.sel', r'.odb.copy' ]
    
            additional_files = [ f.replace( r'__JOB__', j ) for j in job_names for f in [ r'*__JOB__.odb', r'*__JOB__*.sim' ] ]
    
            out_files.extend( [ j+ext for j in job_names for ext in extensions ] )
            out_files.extend( additional_files )
    
            # runMode Configuration
            if solver.runMode == AbaqusRunModes.SEQUENTIAL:
                # Save first and last result ODB files in sequential mode
                odb_result_files = [ SMATsoUtilFile.setExtension( os.path.basename(j), r'.odb' ) for j in driver.FemInput.File ]
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.FIRST_LAST, odb_result_files, r'SAVE.odb', r'_%i', bundle = True )
                # register DELETE on every perturbation, but keep the .com file
                driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, out_files )
                driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.PERT_COMPLETE, EventTimes.NEVER, [ r'*MACZERO*.sim', r'*MACPREV*.sim' ] )
            else:
                # odb files shall not be moved in Simultaneous run mode.
                driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.CYCLE_COMPLETE, EventTimes.NEVER, [ r'*.odb' ], r'SAVE.odb', r'_%i_%p', bundle = True )
    
            # Diagnostics Configuration
            if solver._DiagLevel >= 1 :
                msg_files = [ j+ext for j in job_names for ext in [ r'.msg', r'.msg.*', r'.amg', r'.mgr' ] ]
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.PERT_COMPLETE, EventTimes.FIRST_LAST, msg_files, r'SAVE.msg', r'_%i_%p', bundle = False )
                use_files = [ j+ext for j in job_names for ext in [ r'.use', r'.use.*' ] ]
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.PERT_COMPLETE, EventTimes.FIRST_LAST, use_files, r'SAVE.use', r'_%i_%p', bundle = False )
                dat_files = [ j+ext for j in job_names for ext in [ r'.dat', r'.pre'] ]
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.PERT_COMPLETE, EventTimes.FIRST_LAST, dat_files, r'SAVE.dat', r'_%i_%p', bundle = False )
                sta_files = [ j+r'.sta' for j in job_names ]
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.PERT_COMPLETE, EventTimes.FIRST_LAST, sta_files, r'SAVE.sta', r'_%i_%p', bundle = False )
    
            # delete .com file
            if not "SMA_TSO_ABAQUS_USE_COM_FILE" in os.environ:
                driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, [ r'*.com' ] )
    
            # add the .com file which gets deleted after matrix or cycle is complete
            out_files.extend( [ j+r'.com' for j in job_names ] )
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.OPT_COMPLETE, EventTimes.EVER, out_files )
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.CYCLE_COMPLETE, EventTimes.NEVER, [ r'*MACZERO*.sim', r'*MACPREV*.sim' ] )
    
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.MX_COMPLETE, EventTimes.EVER, out_files )
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.MX_COMPLETE, EventTimes.NEVER, [ r'*MACZERO*.sim', r'*MACPREV*.sim' ] )
    
            _save_inp = r'SAVE.%s'%solver.inputExt
            _list_inp = [ os.path.basename( f ) for f in driver.FemInput.solverInputFiles ]
    
            # When a distribution file is used this SaveRule copies over the initially modified input deck from 000 into each subseqent design cycle folder 001, 002, ...
            # This ensures that the files in each design cycle folder are runable on their own since initially each input deck found in folders 001 and above are only the
            # renamed distribution files and not the complete abaqus model.
            if solver.runMode == AbaqusRunModes.SIMULTANEOUS :
                copy_from_000 = [ os.path.join( driver.WorkDir, _save_inp, r'000', f ) for f in _list_inp ]
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.EVER, copy_from_000, _save_inp , r'_%i', bundle = True )
            # save distribution
            driver.registerSaveRule( UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.EVER, [ solver.DistributionFileSaveName ], _save_inp, r'_%i', bundle = True )
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.OPT_COMPLETE, EventTimes.EVER, [ solver.DistributionFileSaveName ] )
    
            # upon completion of a design cycle save modified input files
            # IMPORTANT: The rule has to be registered AFTER copy_from_000
            # In case of morphing a directory with (-01) is created.
            # This condition avoids the problem.
            if not driver.OptimizationType in { OptimizationTypes.MORPHING_SHAPE, OptimizationTypes.MORPHING_BEAD }:
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.EVER, _list_inp, _save_inp, r'_%i', True )
    
            # disable default SaveRule for saving input files. Otherwise the copy_from_000 SaveRule will be overwritten again and has no effect.
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.CYCLE_COMPLETE, EventTimes.NEVER, _list_inp, _save_inp, r'_%i', True )
    
        # ----------- #
        # -- ANSYS -- #
        # ----------- #
        elif solver.Name == FeSolvers.ANSYS:
            solver.Path = (  driver.FemInput.SolverPath
                          or os.environ.get( r'SMA_TSO_ANSYS_PATH' )
                          or installer_config[ InstallConfig.INS_TSO_ANSYS_PATH ]
                          or ( r'ANSYS181.exe' if isWin else r'ansys' ) )
            solver.MessageExt       = r'erg'
            solver.StandardCallArgs = driver.FemInput.SolverCmdLine or [ r'-b', r'-i', r'__FE_MODEL__.ans', r'-o', r'__FE_MODEL__.erg', r'-s', r'noread', r'-p', r'ansys' ]
            solver.AddCallArgs      = []
            solver.CheckStrings     = [ r'NUMBER OF ERROR   MESSAGES ENCOUNTERED=          0', r'' ]
    
            # -- Environment -- #
    
    
            # Let ansys wait if no license is available
            if os.environ.get( r'ANSWAIT', r'UNDEFINED' ) == r'UNDEFINED':
                os.environ[ r'ANSWAIT' ] = r'1'
    
            # -- SaveRules -- #
    
            delete_on_complete = [ SMATsoUtilFile.setExtension( j, ext ) for j in driver.Solver.JobFiles for ext in [ r'_ans.tosc', r'_ans.solu', r'_ans.head', r'_ans.prep', r'_ans.cdb', r'.db', r'.tmi', r'.ans', r'.s??', r'.stat' ] ]
            delete_on_complete.append( r'file.*' )
    
            intermediate_files = [ SMATsoUtilFile.setExtension( j, ext ) for j in driver.Solver.JobFiles for ext in [ r'.BCS', r'.cdb', r'.mntr', r'.log', r'.full', r'.esav', r'.err' ] ]
            result_files       = [ SMATsoUtilFile.setExtension( j, ext ) for j in driver.Solver.JobFiles for ext in [ r'.rst', r'_*.rst', r'_*.rstp' ] ]
            input_files        = [ SMATsoUtilFile.setExtension( j, ext ) for j in driver.Solver.JobFiles for ext in [ r'.stat', r'.ans', r'.cdb', r'.inp', r'.dat', r'.stat', r'.s??' ] ]
    
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.OPT_COMPLETE, EventTimes.EVER, delete_on_complete )
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, intermediate_files )
    
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.OPT_COMPLETE, EventTimes.EVER, [r'GROUPS.cdb'], r'SAVE.ans' )
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.CYCLE_COMPLETE, EventTimes.FIRST_LAST, [SMATsoUtilFile.setExtension( j, r'.erg' ) for j in solver.JobFiles], r'SAVE.erg', r'_%i' )
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.CYCLE_COMPLETE, EventTimes.FIRST_LAST, result_files, r'SAVE.rst', r'_%i' )
    
            # In case of morphing a directory with (-01) is created.
            # This condition avoids the problem.
            if not driver.OptimizationType in { OptimizationTypes.MORPHING_SHAPE, OptimizationTypes.MORPHING_BEAD }:
                driver.registerSaveRule( UpdateRules.COPY, CheckPoints.PERT_COMPLETE, EventTimes.EVER, input_files, r'SAVE.ans', r'_%i_%p', bundle = True )
    
    
        # ----------------- #
        # -- MSC.NASTRAN -- #
        # ----------------- #
        elif solver.Name == FeSolvers.MSCNASTRAN:
            solver.Path = (  driver.FemInput.SolverPath
                          or os.environ.get( r'SMA_TSO_MSCNASTRAN_PATH' )
                          or installer_config[ InstallConfig.INS_TSO_MSCNASTRAN_PATH ]
                          or ( r'nastran.exe' if isWin else r'mscnastran' ) )
            solver.MessageExt = r'f06'
            if isWin:
                solver.StandardCallArgs = driver.FemInput.SolverCmdLine or [ r'__FE_MODEL_FILE__', r'notify=no', r'old=no', r'scr=yes' ]
            else:
                solver.StandardCallArgs = driver.FemInput.SolverCmdLine or [ r'__FE_MODEL_FILE__', r'notify=no', r'old=no', r'scr=yes', r'bat=no']
    
            solver.AddCallArgs  = []
            solver.CheckStrings = [ r'', r'FATAL MESSAGE' ]
    
            # -- SaveRules -- #
    
            msc_extensions = [ r'.dball', r'.master', r'.f04', r'.op2', r'.f06', r'.log', r'.DAT' ]
    
            delete_files = [ SMATsoUtilFile.setExtension( j, ext ) for j in driver.Solver.JobFiles for ext in msc_extensions ]
            driver.registerSaveRule( UpdateRules.DELETE,
                                     CheckPoints.PERT_COMPLETE,
                                     EventTimes.EVER,
                                     delete_files )
    
            delete_files = [ SMATsoUtilFile.suffixFileName( j, r'_mat', r'tmp' ) for j in solver.JobFiles ]
            driver.registerSaveRule( UpdateRules.DELETE,
                                     CheckPoints.OPT_COMPLETE,
                                     EventTimes.EVER,
                                     delete_files )
    
            copy_files = [ SMATsoUtilFile.setExtension( j, r'.f06' ) for j in solver.JobFiles ]
            driver.registerSaveRule(UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.FIRST_LAST, copy_files, r'SAVE.f06', r'_%i_%p', bundle = True )
    
            copy_files = [ SMATsoUtilFile.setExtension( j, r'.f04' ) for j in solver.JobFiles ]
            driver.registerSaveRule(UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.FIRST_LAST, copy_files, r'SAVE.f04', r'_%i_%p', bundle = True )
    
            copy_files = [ SMATsoUtilFile.setExtension( j, r'.op2' ) for j in solver.JobFiles ]
            driver.registerSaveRule( UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.FIRST_LAST, copy_files, r'SAVE.op2', r'_%i_%p', bundle = True )
    
    
        # ------------ #
        # -- COSMOS -- #
        # ------------ #
        elif solver.Name == FeSolvers.COSMOS and isWin:
            solver.Path = (  driver.FemInput.SolverPath
                          or os.environ.get( r'SMA_TSO_COSMOS_PATH' )
                          or installer_config[ InstallConfig.INS_TSO_COSMOS_PATH ]
                          or r'star.exe' )
            solver.MessageExt       = r'out'
            solver.StandardCallArgs = driver.FemInput.SolverCmdLine or [ r'0', os.path.join( r'F:\Develope\PolaTax\构建\AF5000Ver\镜头总成\镜头桶仿真\MamiyaPressLensSeat-拓扑算例_1-GD',r'__FE_MODEL__' ), r'-silent' ]
            solver.AddCallArgs      = []
            solver.CheckStrings     = [ r'D I S P L A C E M E N T S', r'' ]
    
            # Reporting is unavailable for cosmos
            driver.PostTools.discard( PostTypes.REPORT )
            driver.PostTools.discard( PostTypes.ONF2SIM )
    
            # -- Environment -- #
    
            if r'SMA_TSO_COSMOS_ELEMAT' not in os.environ:
                os.environ[ r'SMA_TSO_COSMOS_ELEMAT' ] = r'1'
    
            # -- SaveRules -- #
    
            cosmos_extensions = [r'.LCD', r'.LOG', r'.STE', r'.VS0', r'.OUT', r'.PCS', r'.TSK', r'.ELM',
                          r'.CNT', r'.DIS', r'.ELF', r'.GP1', r'.IDA', r'.IRF', r'.ITP', r'.LDS',
                          r'.IRF.L??', r'.LCD.L??', r'.LDS.L??', r'.LOD.L??', r'.STE.L??',
                          r'.IRF.B??', r'.LCD.B??', r'.LDS.B??', r'.ITP.B??', r'.STE.B??']
    
            delete_files = [ SMATsoUtilFile.setExtension( j, ext ) for j in driver.Solver.JobFiles for ext in cosmos_extensions ]
            delete_files.extend( [ r'CancelSolverMenu.dat', r'CheckBoxStatus.dat' ] )
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, delete_files )
    
            copy_files = [ SMATsoUtilFile.setExtension( j, r'.OUT' ) for j in driver.Solver.JobFiles]
            driver.registerSaveRule( UpdateRules.COPY, CheckPoints.CYCLE_COMPLETE, EventTimes.FIRST_LAST, copy_files, r'SAVE.OUT', r'_%i_%p', bundle = True )
    
    
    # ------------------------- #
    # LifeSolver Configurations #
    # ------------------------- #
    
    life = driver.LifeSolver
    if life:
    
        # ------------- #
        # -- FE-SAFE -- #
        # ------------- #
        if life.Name == LifeSolvers.FESAFE:
            _work_dir = os.path.join( r'F:\Develope\PolaTax\构建\AF5000Ver\镜头总成\镜头桶仿真\MamiyaPressLensSeat-拓扑算例_1-GD', r'work.fesafe' )
            _batch    = os.path.join( r'F:\Develope\PolaTax\构建\AF5000Ver\镜头总成\镜头桶仿真\MamiyaPressLensSeat-拓扑算例_1-GD', r'__LIFE_MODEL_FILE__' )
            _output   = os.path.join( r'F:\Develope\PolaTax\构建\AF5000Ver\镜头总成\镜头桶仿真\MamiyaPressLensSeat-拓扑算例_1-GD', r'__LIFE_MODEL__.onf' )
    
            life.Path = (  driver.FemInput.LifeSolverPath
                        or os.environ.get( r'SMA_TSO_FESAFE_PATH' )
                        or installer_config[ InstallConfig.INS_TSO_FESAFE_PATH ]
                        or ( r'fe-safe_cl.exe' if isWin else r'fe-safe_cl' ) )
    
            # default fe-safe command line using macro file
            # macro case
            life.StandardCallArgs = driver.FemInput.LifeSolverCmdLine or [ r'-project', _work_dir, r'macro=%s'%_batch ]
            # stlx case
            #life.StandardCallArgs = driver.FemInput.LifeSolverCmdLine or [ r'-project', _work_dir, r'b=%s'%_batch, r'j=refresh', r'mode=surface', r'o=%s'%_output, r'OUTPUT_NODES=1', r'LOGLIVES=0' ]
    
            life.AddCallArgs = driver.FemInput.LifeSolverAddCmd or []
    
            # ensure fe-safe input in work directory
            driver.registerSaveRule( UpdateRules.COPY, CheckPoints.OPT_BEGIN, EventTimes.EVER, [os.path.join(driver.InputDir, pattern) for pattern in [ r'*.ldf', r'*.dac', r'*.kt' ] ], driver.WorkDir )
    
    
             # -- SaveRules -- #
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.PERT_COMPLETE, EventTimes.EVER,
                                     [ r'__LIFE_MODEL___600.onf', r'__LIFE_MODEL___601.onf' ],
                                     r'SAVE.dma', r'_%i', bundle = True )
    
            move_files = [ SMATsoUtilFile.setExtension( j, r'.fer' ) for j in driver.LifeSolver.JobFiles ]
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, move_files, r'SAVE.fer', r'_%i', bundle = False )
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, r'*_life.odb', r'SAVE.odb', r'_%i', bundle = True )
            # multiple log files from fe-safe
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, r'*.log', r'SAVE.log', r'_%i', bundle = False )
    
            delete_files = [ SMATsoUtilFile.setExtension( j, ext ) for j in driver.LifeSolver.JobFiles for ext in [ r'.onf', r'.results', r'.log' ] ]
            driver.registerSaveRule( UpdateRules.DELETE, CheckPoints.PERT_COMPLETE, EventTimes.EVER, delete_files )
    
        # ------------ #
        # -- FEMFAT -- #
        # ------------ #
        if life.Name == LifeSolvers.FEMFAT:
            life.Path = (  driver.FemInput.LifeSolverPath
                        or os.environ.get( r'SMA_TSO_FEMFAT_PATH' )
                        or installer_config[ InstallConfig.INS_TSO_FEMFAT_PATH ]
                        or ( r'femfat.bat' if isWin else r'femfat' ) )
            life.StandardCallArgs = driver.FemInput.LifeSolverCmdLine or [ r'-job=__LIFE_MODEL__.ffj' ]
            life.AddCallArgs = driver.FemInput.LifeSolverAddCmd or []
    
            # invert femfat results
            life.Invert = True
    
            # -- SaveRules -- #
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.PERT_COMPLETE, EventTimes.EVER,
                                     [ r'__LIFE_MODEL___600.onf', r'__LIFE_MODEL___601.onf', r'__LIFE_MODEL__.dma' ],
                                     r'SAVE.dma', r'_%i', bundle = True )
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.PERT_COMPLETE, EventTimes.EVER,
                                     [ r'__LIFE_MODEL__.pro' ], r'SAVE.pro' )
            driver.registerSaveRule( UpdateRules.MOVE, CheckPoints.PERT_COMPLETE, EventTimes.EVER,
                                     [ r'__LIFE_MODEL__.fps' ], r'SAVE.fps' )
    

    pass

def config_214509548703_1(config, driver, installer_config):
  source_file = r'DRIVER command of F:\Develope\PolaTax\构建\AF5000Ver\镜头总成\镜头桶仿真\MamiyaPressLensSeat-拓扑算例_1-GD.par'

  os.environ[r'SMA_TSO_COSMOS_ELEMAT'] = r'1'
  driver.registerSaveRule( UpdateRules.COPY, CheckPoints.ITER_COMPLETE, EventTimes.LAST, [ r'*.ste*' ], r'SAVE.ste', r'' )
  driver.registerSaveRule( UpdateRules.COPY, CheckPoints.ITER_COMPLETE, EventTimes.LAST, [ r'*.lcd*' ], r'SAVE.ste', r'' )
  driver.registerSaveRule( UpdateRules.COPY, CheckPoints.ITER_COMPLETE, EventTimes.LAST, [ r'*.lcm*' ], r'SAVE.ste', r'' )

  pass
